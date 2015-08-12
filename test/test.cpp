#include <iostream>
#include <future>
#include <chrono>
#include <vector>
#include <string>
#include <queue>

std::string download_url()
{
    std::this_thread::sleep_for(std::chrono::milliseconds( 1000 ));
    return "hello world";
}

class processor
{
public:
	processor(size_t);
	~processor();
	processor(const processor&) = delete;
	processor& operator=(const processor&) = delete;
	
	void enqueue(std::shared_ptr< std::packaged_task<void()> > func);
	
private:
	// need to keep track of threads so we can join them
	std::vector< std::thread > _workers;
	// the task queue
	std::queue< std::shared_ptr< std::packaged_task<void()> > > _tasks;
	
	// synchronization
	std::mutex _queue_mutex;
	std::condition_variable _condition;
	bool _stop;
};

processor::processor(size_t threads) : _stop(false)
{
	for (size_t i = 0; i < threads; ++i)
	{
		_workers.emplace_back(
			[this]
		{
			for (;;)
			{
				std::shared_ptr< std::packaged_task<void()> > task;

				{
					std::unique_lock<std::mutex> lock(this->_queue_mutex);
					this->_condition.wait(lock, [this]{ return this->_stop || !this->_tasks.empty(); });
					if (this->_stop && this->_tasks.empty())
					{
						return;
					}
					task = std::move(this->_tasks.front());
					this->_tasks.pop();
				}

				(*task)();
			}
		}
		);
	}
}
void processor::enqueue(std::shared_ptr< std::packaged_task<void()> > func)
{
	{
		std::unique_lock<std::mutex> lock(_queue_mutex);

		if (!_stop)
		{
			_tasks.emplace(func);
		}
	}
	_condition.notify_one();
}

// the destructor joins all threads
processor::~processor()
{
	{
		std::unique_lock<std::mutex> lock(_queue_mutex);
		_stop = true;
	}
	_condition.notify_all();

	for (auto &worker : _workers)
	{
		worker.join();
	}
}

static processor p(4);

template< typename Function>
std::future<typename std::result_of<Function()>::type> async_lite(Function&& f)
{
	auto packaged_pack_external = std::make_shared< std::packaged_task<Function()> >( std::bind(std::forward<Function>(f)) );
	
	auto task = std::make_shared< std::packaged_task<void()> >(
		[=]() {
			(*packaged_pack_external)( );
		});
	p.enqueue(task);
	
	return packaged_pack_external->get_future();
	
	//return std::async(std::launch::async, std::forward<Function>(f), std::forward<Args>(args)...);
}

template< typename Function, typename Then>
std::future<typename std::result_of<Function()>::type> await(Function&& f, Then&& t)
{
    return async_lite(
        [&]() -> typename std::result_of<Function()>::type
        {
            auto result = async_lite(std::forward<Function>(f));
            auto result_real = result.get();
            t(result_real);
            return result_real;
        });
}

int main()
{
	std::vector<std::future<std::string> > _results;
	
    for(int i=0; i<20; ++i)
    {
		_results.emplace_back(
			await(
					download_url, [](const std::string& result)
					{
						std::cout << "Result is " << result << std::endl;
					}
				)
		);
    }
    return(0);
}
