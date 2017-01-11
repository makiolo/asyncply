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
	using post_type = std::function<R(const R&)>;

	template <typename Function, typename ... Args>
	task(Function&& f, Args&& ... args)
		: _result( asyncply::_async(std::forward<Function>(f), std::forward<Args>(args)...) )
	{ ; }

	~task() { ; }

	task(const task&) = delete;
	task& operator=(const task&) = delete;

	template <typename Function>
	task_t<R> then(typename std::enable_if<(!std::is_void<typename std::result_of<Function(R)>::type>::value), Function>::type&& post_method)
	{
		task_t<R> this_task = this->shared_from_this();
		return asyncply::async(
					[this_task](Function&& post_method){
						return post_method(this_task->get());
					},
					std::forward<Function>(post_method)
		);
	}
	
	template <typename Function>
	task_t<R> then(typename std::enable_if<(std::is_void<typename std::result_of<Function(R)>::type>::value), Function>::type&& post_method)
	{
		task_t<R> this_task = this->shared_from_this();
		return asyncply::async(
					[this_task](Function&& post_method){
						this_task->get();
						post_method();
					},
					std::forward<Function>(post_method)
		);
	}

	R get()
	{
		return _result.get();
	}

protected:
	std::future<R> _result;
};

}

#endif
