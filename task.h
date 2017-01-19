#ifndef _TASK_H_
#define _TASK_H_

#include <memory>
#include <functional>
#include <thread>
#include "future.h"
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
		: _result(std::forward<TaskFuture<R> >(asyncply::_async(std::forward<Function>(f), std::forward<Args>(args)...) ) )
		, _last(true)
	{ ; }
	~task() {
		if(_last) get();
	}

	task(const task&) = delete;
	task& operator=(const task&) = delete;

	template <typename Function>
	task_t<R> then(Function&& post_method)
	{
		_last = false;
		task_t<R> this_task = this->shared_from_this();
		_then_task = asyncply::async(
			[this_task, post_method = std::move(post_method)](){
				return_type r = this_task->get();
				return post_method(r);
			}
		);
		return _then_task;
	}

	auto get(void)
	{
		if(valid())
		{
			_value = _result.get();
		}
		return _value;
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
	TaskFuture<R> _result;
	task_t<R> _then_task;
	bool _last;
	R _value;
};

template <>
class task<void> : public std::enable_shared_from_this<task<void> >
{
public:
	using return_type = void;
	
	template <typename Function, typename ... Args>
	task(Function&& f, Args&& ... args)
		: _result( std::forward<TaskFuture<void> >(asyncply::_async(std::forward<Function>(f), std::forward<Args>(args)...) ) )
		, _last(true)
	{ ; }
	~task() {
		if(_last) get();
	}

	task(const task&) = delete;
	task& operator=(const task&) = delete;

	template <typename Function>
	task_t<void> then(Function&& post_method)
	{
		_last = false;
		task_t<void> this_task = this->shared_from_this();
		_then_task = asyncply::async(
			[this_task, post_method = std::move(post_method)]() {
				this_task->get();
				post_method();
			}
		);
		return _then_task;
	}

	void get()
	{
		if(valid())
		{
			_result.get();
		}
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
	TaskFuture<void> _result;
	task_t<void> _then_task;
	bool _last;
};

}

#endif
