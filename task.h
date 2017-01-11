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
	using func = std::function<R()>;
	using return_type = R;
	using post_type = std::function<return_type(const return_type&)>;

	template <typename Function, typename ... Args>
	task(Function&& f, Args&& ... args)
		: _result( asyncply::_async(std::forward<Function>(f), std::forward<Args>(args)...) )
	{ ; }

	~task() { ; }

	task(const task&) = delete;
	task& operator=(const task&) = delete;

	template <typename Function>
	task_t<return_type> then(Function&& post_method)
	{
		task_t<return_type> this_task = this->shared_from_this();
		return asyncply::async(
					[this_task](Function&& post_method){
						return post_method(this_task->get());
					},
					std::forward<Function>(post_method)
		);
	}

	return_type get()
	{
		return _result.get();
	}

protected:
	std::future<return_type> _result;
};

template <>
class task<void> : public std::enable_shared_from_this<task<void> >
{
public:
	using func = std::function<void()>;
	using return_type = void;
	using post_type = std::function<return_type()>;

	template <typename Function, typename ... Args>
	task(Function&& f, Args&& ... args)
		: _result( asyncply::_async(std::forward<Function>(f), std::forward<Args>(args)...) )
	{ ; }

	~task() { ; }

	task(const task& te) = delete;
	task& operator=(const task& te) = delete;
	
	template <typename Function>
	task_t<return_type> then(Function&& post_method)
	{
		task_t<return_type> this_task = this->shared_from_this();
		return asyncply::async(
					[this_task](Function&& post_method){
						this_task->get();
						post_method();
					},
					std::forward<Function>(post_method)
		);
	}

	void get()
	{
		_result.get();
	}

protected:
	std::future<void> _result;
};

}

#endif
