#include <iostream>
#include <future>
#include <chrono>
#include <vector>
#include <string>
#include <queue>

std::string download_url(const std::string& echo)
{
    std::this_thread::sleep_for(std::chrono::milliseconds( 1000 ));
    return echo;
}

class processor
{
public:
	processor(size_t);
	~processor();
	processor(const processor&) = delete;
	processor& operator=(const processor&) = delete;
	
	void enqueue(const std::shared_ptr< std::packaged_task<void()> >& func);
	
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
void processor::enqueue(const std::shared_ptr< std::packaged_task<void()> >& func)
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

template< typename Function, typename... Args>
std::future<typename std::result_of<Function(Args&&...)>::type> async_lite(Function&& f, Args&&... args)
{
	/*
	auto packaged_pack_internal = std::make_shared< std::packaged_task<void()> >( std::bind(f, args...) );
	p.enqueue(packaged_pack_internal);
	return packaged_pack_internal->get_future();
	*/
	return std::async(std::launch::async, std::forward<Function>(f), std::forward<Args>(args)...);
}

template< typename Function, typename Then, typename... Args>
std::future<typename std::result_of<Function(Args...)>::type> await(Function&& f, Then&& t, Args&&... args)
{
    return async_lite(
        [&](Function&& ff, Then&& tt, Args&& ... argss) -> typename std::result_of<Function(Args...)>::type
        {
            auto result = async_lite(std::forward<Function>(ff), std::forward<Args>(argss)...);
            auto result_real = result.get();
            tt(result_real);
            return result_real;
        }, std::forward<Function>(f), std::forward<Then>(t), std::forward<Args>(args)...);
}

int main()
{
    for(int i=0; i<20; ++i)
    {
        await(
                std::bind(download_url, "ecooo66"), [](const std::string& result)
                {
                    std::cout << "Result is " << result << std::endl;
                }
            );
    }
    return(0);
}
