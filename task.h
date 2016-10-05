#ifndef _TASK_H_
#define _TASK_H_

#include <functional>
#include <thread>
#include "run_fwd.h"
#include "async.h"

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
		asyncply::async(_result, [&](){
			return _method();
		});
	}

	task(const func& method, const post_type& post_method)
		: task(method)
	{
		then(post_method);
	}

	~task() { ; }

	task(const task&) = delete;
	task& operator=(const task&) = delete;

	void then(const post_type& post_method)
	{
		if(has_post())
		{
			throw std::exception();
		}
		else
		{
			_has_post = true;
			_post_method = post_method;
			asyncply::async(_result_post, [&](){
				return _post_method(_result.get());
			});
		}
	}

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
		asyncply::async(_result, [&](){
			_method();
		});
	}

	task(const func& method, const post_type& post_method)
		: task(method)
	{
		then(post_method);
	}

	~task() { ; }

	task(const task& te) = delete;
	task& operator=(const task& te) = delete;

	void then(const post_type& post_method)
	{
		if(has_post())
		{
			throw std::exception();
		}
		else
		{
			_has_post = true;
			_post_method = post_method;
			asyncply::async(_result_post, [&](){
				_result.get();
				_post_method();
			});
		}
	}

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
};

}

#endif

