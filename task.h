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

	task(const func& method)
		: _method(method)
		, _result(	asyncply::_async([&]() {
					return method();
				} ) )
	{ ; }

	~task() { ; }

	task(const task&) = delete;
	task& operator=(const task&) = delete;

	task_t<return_type> then(const post_type& post_method)
	{
		task_t<return_type> this_task = shared_from_this();
		return asyncply::async([this_task](){
			return post_method(this_task->get());
		});
	}

	return_type get()
	{
		return _result.get();
	}

protected:
	func _method;
	std::future<return_type> _result;
};

template <>
class task<void> : public std::enable_shared_from_this<task<void> >
{
public:
	using func = std::function<void()>;
	using return_type = void;
	using post_type = std::function<return_type()>;

	task(const func& method)
		: _method(method)
		, _result(	asyncply::_async([&](){
					method();
				} ) )
	{ ; }

	~task() { ; }

	task(const task& te) = delete;
	task& operator=(const task& te) = delete;

	task_t<return_type> then(const post_type& post_method)
	{
		task_t<return_type> this_task = shared_from_this();
		return asyncply::async([this_task](){
			this_task->get();
			post_method();
		});
	}

	void get()
	{
		_result.get();
	}

protected:
	func _method;
	std::future<void> _result;
};

}

#endif
