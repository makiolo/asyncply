#ifndef _TASK_H_
#define _TASK_H_

#include <functional>
#include "run_fwd.h"
#include "future.h"
#include <thread>
//
#include <thread_pool.hpp>

static ThreadPool pool;

namespace asyncply {

template <typename R>
class task
{
public:
	using func = std::function<R()>;
	using return_type = R;
	using post_type = std::function<return_type(const return_type&)>;

	task(const func& method)
		: _method(method)
		, _post_method(nullptr)
		, _has_post(false)
	{
        std::packaged_task<return_type()> t([&](){
			return _method();
        });
        _result = t.get_future();

		// add in pool
        pool.post(t);
	}

	task(const func& method, const post_type& post_method)
		: task(method)
		// , _method(method)
		// , _post_method(post_method)
		// , _has_post(true)
	{
		_post_method = post_method;
		_has_post = true;

        std::packaged_task<return_type()> t([&](){
			return _post_method(_result.get());
        });
        _result_post = t.get_future();

		// add in pool
        pool.post(t);
	}

	~task() { ; }

	task(const task&) = delete;
	task& operator=(const task&) = delete;

	return_type get()
	{
		if(has_post())
		{
			return _result_post.get();
		}
		else
		{
			return _result.get();
		}
	}

	inline bool has_post() const { return (_has_post && _post_method); }

protected:
	func _method;
	post_type _post_method;
	std::future<return_type> _result;
	std::future<return_type> _result_post;
	bool _has_post;
};

template <>
class task<void>
{
public:
	using func = std::function<void()>;
	using return_type = void;
	using post_type = std::function<return_type()>;

	task(const func& method)
		: _method(method)
        , _post_method()
		, _has_post(false)
	{
        std::packaged_task<return_type()> t([&](){
			_method();
        });
        _result = t.get_future();

		// add in pool
        pool.post(t);
	}

	task(const func& method, const post_type& post_method)
		: task(method)
		// , _method(method)
		// , _post_method(post_method)
		// , _has_post(true)
	{
		_post_method = post_method;
		_has_post = true;

        std::packaged_task<return_type()> t([&](){
			_result.get();
			_post_method();
        });
        _result_post = t.get_future();

		// add in pool
        pool.post(t);
	}

	~task() { ; }

	task(const task& te) = delete;
	task& operator=(const task& te) = delete;

	void get()
	{
		if(has_post())
		{
			_result_post.get();
		}
		else
		{
			_result.get();
		}
	}

	inline bool has_post() const { return (_has_post && _post_method); }

protected:
	func _method;
	post_type _post_method;
	std::future<void> _result;
	std::future<void> _result_post;
	bool _has_post;
	// uv_work_t _req;
};

}

#endif

