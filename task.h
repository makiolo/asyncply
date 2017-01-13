#ifndef _TASK_H_
#define _TASK_H_

#include <memory>
#include <functional>
#include <thread>
#include "run_fwd.h"
#include "async.h"

namespace asyncply {

template <typename R>
class task : public std::enable_shared_from_this<task<R> >
{
public:
	using return_type = R;
	
	template <typename Function, typename ... Args>
	task(Function&& f, Args&& ... args)
		: _result( asyncply::_async(std::forward<Function>(f), std::forward<Args>(args)...) ) { ; }
	~task() { ; }

	task(const task&) = delete;
	task& operator=(const task&) = delete;

	template <typename Function, typename ... Args>
	task_t<R> then(Function&& post_method, Args&& ... args)
	{
		task_t<R> this_task = this->shared_from_this();
		return asyncply::async(
			[this_task](Function&& post_method, Args&& ... args){
				auto ff = std::bind(std::forward<Function>(post_method), std::forward<Args>(args)...);
				return ff(this_task->get());
			},
			std::forward<Function>(post_method), std::forward<Args>(args)...
		);
	}

	R get()
	{
		return _result.get();
	}

	inline bool valid() const
	{
		return _result.valid();
	}

	inline void wait() const
	{
		_result.wait();
	}

	template< class Rep, class Period >
	inline std::future_status wait_for( const std::chrono::duration<Rep,Period>& timeout_duration ) const
	{
		return _result.wait_for(timeout_duration);
	}

	template< class Clock, class Duration >
	inline std::future_status wait_until( const std::chrono::time_point<Clock,Duration>& timeout_time ) const
	{
		return _result.wait_until(timeout_time);
	}
	
	inline bool is_ready() const
	{
		return wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	}
	
protected:
	std::future<R> _result;
};

template <>
class task<void> : public std::enable_shared_from_this<task<void> >
{
public:
	using return_type = void;
	
	template <typename Function, typename ... Args>
	task(Function&& f, Args&& ... args)
		: _result( asyncply::_async(std::forward<Function>(f), std::forward<Args>(args)...) ) { ; }
	~task() { ; }

	task(const task&) = delete;
	task& operator=(const task&) = delete;

	template <typename Function, typename ... Args>
	task_t<void> then(Function&& post_method, Args&& ... args)
	{
		task_t<void> this_task = this->shared_from_this();
		return asyncply::async(
			[this_task](Function&& post_method, Args&& ... args) {
				auto ff = std::bind(std::forward<Function>(post_method), std::forward<Args>(args)...);
				this_task->get();
				ff();
			},
			std::forward<Function>(post_method), std::forward<Args>(args)...
		);
	}

	void get()
	{
		_result.get();
	}

	inline bool valid() const
	{
		return _result.valid();
	}

	inline void wait() const
	{
		_result.wait();
	}

	template< class Rep, class Period >
	inline std::future_status wait_for( const std::chrono::duration<Rep,Period>& timeout_duration ) const
	{
		return _result.wait_for(timeout_duration);
	}

	template <class Clock, class Duration>
	inline std::future_status wait_until( const std::chrono::time_point<Clock,Duration>& timeout_time ) const
	{
		return _result.wait_until(timeout_time);
	}

	inline bool is_ready() const
	{
		return wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	}

protected:
	std::future<void> _result;
};

}

#endif
