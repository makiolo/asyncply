#ifndef _RUN_H_
#define _RUN_H_

#include <thread>
#include <iostream>
#include <memory>
#include <algorithm>
#include <atomic>
#include <cstdint>
#include <functional>
#include <future>
#include <type_traits>
#include <utility>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <teelogging/teelogging.h>
#include "metacommon/common.h"
#include "run_fwd.h"
#include "task.h"

// sfinae modern
// https://jguegant.github.io/blogs/tech/sfinae-introduction.html

#ifndef __ctpl_thread_pool_H__
#define __ctpl_thread_pool_H__
/*********************************************************
 *
 *  Copyright (C) 2014 by Vitaliy Vitsentiy
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *********************************************************/

#include <functional>
#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include <exception>
#include <future>
#include <mutex>
#include <boost/lockfree/queue.hpp>

#ifndef _ctplThreadPoolLength_
#define _ctplThreadPoolLength_  100
#endif

// thread pool to run user's functors with signature
//      ret func(int id, other_params)
// where id is the index of the thread that runs the functor
// ret is some return type

namespace ctpl {

    class thread_pool {

    public:

        thread_pool() : q(_ctplThreadPoolLength_) { this->init(); }
        thread_pool(int nThreads, int queueSize = _ctplThreadPoolLength_) : q(queueSize) { this->init(); this->resize(nThreads); }

        // the destructor waits for all the functions in the queue to be finished
        ~thread_pool() {
            this->stop(true);
        }

        // get the number of running threads in the pool
        int size() { return static_cast<int>(this->threads.size()); }

        // number of idle threads
        int n_idle() { return this->nWaiting; }
        std::thread & get_thread(int i) { return *this->threads[i]; }

        // change the number of threads in the pool
        // should be called from one thread, otherwise be careful to not interleave, also with this->stop()
        // nThreads must be >= 0
        void resize(int nThreads) {
            if (!this->isStop && !this->isDone) {
                int oldNThreads = static_cast<int>(this->threads.size());
                if (oldNThreads <= nThreads) {  // if the number of threads is increased
                    this->threads.resize(nThreads);
                    this->flags.resize(nThreads);

                    for (int i = oldNThreads; i < nThreads; ++i) {
                        this->flags[i] = std::make_shared<std::atomic<bool>>(false);
                        this->set_thread(i);
                    }
                }
                else {  // the number of threads is decreased
                    for (int i = oldNThreads - 1; i >= nThreads; --i) {
                        *this->flags[i] = true;  // this thread will finish
                        this->threads[i]->detach();
                    }
                    {
                        // stop the detached threads that were waiting
                        std::unique_lock<std::mutex> lock(this->mutex);
                        this->cv.notify_all();
                    }
                    this->threads.resize(nThreads);  // safe to delete because the threads are detached
                    this->flags.resize(nThreads);  // safe to delete because the threads have copies of shared_ptr of the flags, not originals
                }
            }
        }

        // empty the queue
        void clear_queue() {
            std::function<void(int id)> * _f;
            while (this->q.pop(_f))
                delete _f;  // empty the queue
        }

        // pops a functional wraper to the original function
        std::function<void(int)> pop() {
            std::function<void(int id)> * _f = nullptr;
            this->q.pop(_f);
            std::unique_ptr<std::function<void(int id)>> func(_f);  // at return, delete the function even if an exception occurred
            
            std::function<void(int)> f;
            if (_f)
                f = *_f;
            return f;
        }


        // wait for all computing threads to finish and stop all threads
        // may be called asyncronously to not pause the calling thread while waiting
        // if isWait == true, all the functions in the queue are run, otherwise the queue is cleared without running the functions
        void stop(bool isWait = false) {
            if (!isWait) {
                if (this->isStop)
                    return;
                this->isStop = true;
                for (int i = 0, n = this->size(); i < n; ++i) {
                    *this->flags[i] = true;  // command the threads to stop
                }
                this->clear_queue();  // empty the queue
            }
            else {
                if (this->isDone || this->isStop)
                    return;
                this->isDone = true;  // give the waiting threads a command to finish
            }
            {
                std::unique_lock<std::mutex> lock(this->mutex);
                this->cv.notify_all();  // stop all waiting threads
            }
            for (int i = 0; i < static_cast<int>(this->threads.size()); ++i) {  // wait for the computing threads to finish
                if (this->threads[i]->joinable())
                    this->threads[i]->join();
            }
            // if there were no threads in the pool but some functors in the queue, the functors are not deleted by the threads
            // therefore delete them here
            this->clear_queue();
            this->threads.clear();
            this->flags.clear();
        }

        template<typename F, typename... Rest>
        auto push(F&& f, Rest&&... rest) ->std::future< asyncply::result_type<F, int, Rest...> > {
            auto pck = std::make_shared<std::packaged_task< asyncply::result_type<F, int, Rest...>(int)> >(
                std::bind(std::forward<F>(f), std::placeholders::_1, std::forward<Rest>(rest)...)
            );

            auto _f = new std::function<void(int id)>([pck](int id) {
                (*pck)(id);
            });
            this->q.push(_f);

            std::unique_lock<std::mutex> lock(this->mutex);
            this->cv.notify_one();

            return pck->get_future();
        }

        // run the user's function that excepts argument int - id of the running thread. returned value is templatized
        // operator returns std::future, where the user can get the result and rethrow the catched exceptins
        template<typename F>
        auto push(F&& f) -> std::future< asyncply::result_type<F, int> > {
            auto pck = std::make_shared<std::packaged_task< asyncply::result_type<F, int>(int)>>(std::forward<F>(f));

            auto _f = new std::function<void(int id)>([pck](int id) {
                (*pck)(id);
            });
            this->q.push(_f);

            std::unique_lock<std::mutex> lock(this->mutex);
            this->cv.notify_one();

            return pck->get_future();
        }


    private:

        // deleted
        thread_pool(const thread_pool &);// = delete;
        thread_pool(thread_pool &&);// = delete;
        thread_pool & operator=(const thread_pool &);// = delete;
        thread_pool & operator=(thread_pool &&);// = delete;

        void set_thread(int i) {
            std::shared_ptr<std::atomic<bool>> flag(this->flags[i]);  // a copy of the shared ptr to the flag
            auto f = [this, i, flag/* a copy of the shared ptr to the flag */]() {
                std::atomic<bool> & _flag = *flag;
                std::function<void(int id)> * _f;
                bool isPop = this->q.pop(_f);
                while (true) {
                    while (isPop) {  // if there is anything in the queue
                        std::unique_ptr<std::function<void(int id)>> func(_f);  // at return, delete the function even if an exception occurred
                        (*_f)(i);

                        if (_flag)
                            return;  // the thread is wanted to stop, return even if the queue is not empty yet
                        else
                            isPop = this->q.pop(_f);
                    }

                    // the queue is empty here, wait for the next command
                    std::unique_lock<std::mutex> lock(this->mutex);
                    ++this->nWaiting;
                    this->cv.wait(lock, [this, &_f, &isPop, &_flag](){ isPop = this->q.pop(_f); return isPop || this->isDone || _flag; });
                    --this->nWaiting;

                    if (!isPop)
                        return;  // if the queue is empty and this->isDone == true or *flag then return
                }
            };
            this->threads[i].reset(new std::thread(f));  // compiler may not support std::make_unique()
        }

        void init() { this->nWaiting = 0; this->isStop = false; this->isDone = false; }

        std::vector<std::unique_ptr<std::thread>> threads;
        std::vector<std::shared_ptr<std::atomic<bool>>> flags;
        mutable boost::lockfree::queue<std::function<void(int id)> *> q;
        std::atomic<bool> isDone;
        std::atomic<bool> isStop;
        std::atomic<int> nWaiting;  // how many threads are waiting

        std::mutex mutex;
        std::condition_variable cv;
    };

}

#endif // __ctpl_thread_pool_H__

 
// http://roar11.com/2016/01/a-platform-independent-thread-pool-using-c14/
namespace asyncply
{
	
template <typename T>
class ThreadSafeQueue
{
public:
/**
 * Destructor.
 */
~ThreadSafeQueue(void)
{
	LOGV("ThreadSafeQueue::destructor");
	invalidate();
}

/**
 * Attempt to get the first value in the queue.
 * Returns true if a value was successfully written to the out parameter, false otherwise.
 */
bool tryPop(T& out)
{
	std::lock_guard<std::mutex> lock{m_mutex};
	if(m_queue.empty() || !m_valid)
	{
		return false;
	}
	LOGV("ThreadSafeQueue::tryPop");
	out = std::move(m_queue.front());
	m_queue.pop();
	return true;
}

/**
 * Get the first value in the queue.
 * Will block until a value is available unless clear is called or the instance is destructed.
 * Returns true if a value was successfully written to the out parameter, false otherwise.
 */
bool waitPop(T& out)
{
    std::unique_lock<std::mutex> lock{m_mutex};
    m_condition.wait(lock, [this]()
    {
	return !m_queue.empty() || !m_valid;
    });
    /*
     * Using the condition in the predicate ensures that spurious wakeups with a valid
     * but empty queue will not proceed, so only need to check for validity before proceeding.
     */
    if(!m_valid)
    {
	return false;
    }
	LOGV("ThreadSafeQueue::waitPop");
    out = std::move(m_queue.front());
    m_queue.pop();
    return true;
}

/**
 * Push a new value onto the queue.
 */
void push(T value)
{
	std::lock_guard<std::mutex> lock{m_mutex};
	LOGV("ThreadSafeQueue::push");
	m_queue.push(std::move(value));
	m_condition.notify_one();
}

/**
 * Check whether or not the queue is empty.
 */
bool empty(void) const
{
	std::lock_guard<std::mutex> lock{m_mutex};
	LOGV("ThreadSafeQueue::empty");
	return m_queue.empty();
}

/**
 * Clear all items from the queue.
 */
void clear(void)
{
	std::lock_guard<std::mutex> lock{m_mutex};
	while(!m_queue.empty())
	{
		m_queue.pop();
	}
	m_condition.notify_all();
	LOGV("ThreadSafeQueue::clear");
}

/**
 * Invalidate the queue.
 * Used to ensure no conditions are being waited on in waitPop when
 * a thread or the application is trying to exit.
 * The queue is invalid after calling this method and it is an error
 * to continue using a queue after this method has been called.
 */
void invalidate(void)
{
	std::lock_guard<std::mutex> lock{m_mutex};
	LOGV("ThreadSafeQueue::invalidate");
	m_valid = false;
	m_condition.notify_all();
}

/**
 * Returns whether or not this queue is valid.
 */
bool isValid(void) const
{
	std::lock_guard<std::mutex> lock{m_mutex};
	LOGV("ThreadSafeQueue::isValid");
	return m_valid;
}

private:
	std::atomic_bool m_valid{true};
	mutable std::mutex m_mutex;
	std::queue<T> m_queue;
	std::condition_variable m_condition;
};

class ThreadPool
{
private:
	class IThreadTask
	{
	public:
		IThreadTask(void) = default;
		virtual ~IThreadTask(void) = default;
		IThreadTask(const IThreadTask& rhs) = delete;
		IThreadTask& operator=(const IThreadTask& rhs) = delete;
		IThreadTask(IThreadTask&& other) = default;
		IThreadTask& operator=(IThreadTask&& other) = default;

		/**
		* Run the task.
		*/
		virtual void execute() = 0;
	};

	template <typename Func>
	class ThreadTask: public IThreadTask
	{
	public:
	    ThreadTask(Func&& func)
		:m_func{std::move(func)}
	    {
		    LOGV("ThreadTask::constructor");
	    }

	    ~ThreadTask(void) override = default;
	    ThreadTask(const ThreadTask& rhs) = delete;
	    ThreadTask& operator=(const ThreadTask& rhs) = delete;
	    ThreadTask(ThreadTask&& other) = default;
	    ThreadTask& operator=(ThreadTask&& other) = default;

	    /**
	     * Run the task.
	     */
	    void execute() override
	    {
		LOGV("ThreadTask::execute begin");
		m_func();
		LOGV("ThreadTask::execute end");
	    }

	private:
	    Func m_func;
	};

public:
/**
 * Constructor.
 */
ThreadPool(void)
    :ThreadPool{std::max(std::thread::hardware_concurrency(), 2u) - 1u}
{
	/*
	* Always create at least one thread.  If hardware_concurrency() returns 0,
	* subtracting one would turn it to UINT_MAX, so get the maximum of
	* hardware_concurrency() and 2 before subtracting 1.
	*/
	LOGV("ThreadPool::constructor");
}

/**
 * Constructor.
 */
explicit ThreadPool(const std::uint32_t numThreads)
    :m_done{false},
    m_workQueue{},
    m_threads{}
{
    try
    {
	for(std::uint32_t i = 0u; i < numThreads; ++i)
	{
	    m_threads.emplace_back(&ThreadPool::worker, this);
	}
    }
    catch(...)
    {
	LOGV("ThreadPool::exception");
	destroy();
	throw;
    }
}

/**
 * Non-copyable.
 */
ThreadPool(const ThreadPool& rhs) = delete;

/**
 * Non-assignable.
 */
ThreadPool& operator=(const ThreadPool& rhs) = delete;

/**
 * Destructor.
 */
~ThreadPool(void)
{
    destroy();
}

/**
 * Submit a job to be run by the thread pool.
 */
template <typename Func, typename... Args>	
auto submit(Func&& func, Args&&... args) -> std::enable_if_t<(!std::is_void< result_type<Func, Args...> >::value), future_of_functor<Func, Args...> >
{
	LOGV("ThreadPool::submit (non-void)");
	
	auto boundTask = std::bind< result_type<Func, Args...> >(std::forward<Func>(func), std::forward<Args>(args)...);
	using PackagedTask = std::packaged_task< result_type<Func, Args...>() >;
	using TaskType = ThreadTask<PackagedTask>;

	static_assert(std::is_same< result_type<Func, Args...>, decltype(boundTask()) >::value, "error in is_same");
	static_assert(std::is_same< future_of_functor<Func, Args...>, TaskFuture< result_type<Func, Args...> > >::value, "error in is_same");
	
	PackagedTask task( std::move(boundTask) );
	future_of_functor<Func, Args...> result( task.get_future() );
	m_workQueue.push( std::make_unique<TaskType>(std::move(task)) );
	return result;
}

template <typename Func, typename... Args>	
auto submit(Func&& func, Args&&... args) -> std::enable_if_t<(std::is_void< result_type<Func, Args...> >::value), TaskFuture<void> >
{
	LOGV("ThreadPool::submit (void)");
	
	auto boundTask = std::bind<void>(std::forward<Func>(func), std::forward<Args>(args)...);
	using PackagedTask = std::packaged_task<void()>;
	using TaskType = ThreadTask<PackagedTask>;

	PackagedTask task( std::move(boundTask) );
	TaskFuture<void> result( task.get_future() );
	m_workQueue.push( std::make_unique<TaskType>(std::move(task)) );
	return result;
}
	
private:
/**
 * Constantly running function each thread uses to acquire work items from the queue.
 */
void worker(void)
{
    while(!m_done)
    {
		LOGV("ThreadPool::working ...");
		std::unique_ptr<IThreadTask> pTask{nullptr};
		if(m_workQueue.waitPop(pTask))
		{
			pTask->execute();
		}
    }
}

/**
 * Invalidates the queue and joins all running threads.
 */
void destroy(void)
{
	m_done = true;
	m_workQueue.invalidate();
	for(auto& thread : m_threads)
	{
		if(thread.joinable())
		{
			thread.join();
		}
	}
	LOGV("ThreadPool::joined workers");
}

private:
	std::atomic_bool m_done;
	ThreadSafeQueue<std::unique_ptr<IThreadTask>> m_workQueue;
	std::vector<std::thread> m_threads;
};

/**
 * Get the default thread pool for the application.
 * This pool is created with std::thread::hardware_concurrency() - 1 threads.
 */
asyncply::thread_pool& getThreadPool(void);

template <typename Function, typename ... Args>
auto __async(Function&& f, Args&& ... args) -> future_of_functor<Function, Args...>
{
	LOGV("ThreadPool::__async");
	// return getThreadPool().submit(std::forward<Function>(f), std::forward<Args>(args)...);
	ctpl::thread_pool& pool = getThreadPool();
	return pool.push( std::bind(std::forward<Function>(f), std::forward<Args>(args)...) );
}

template <typename Function, typename ... Args>
auto async(Function&& f, Args&& ... args) -> shared_task<Function, Args...>
{
	LOGV("ThreadPool::async");
	return std::make_shared< task_of_functor<Function, Args...> >(std::forward<Function>(f), std::forward<Args>(args)...);
}

}

#endif
