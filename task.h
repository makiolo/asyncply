#ifndef _TASK_H_
#define _TASK_H_

#include <functional>
#include <uv.h>
#include "promise.h"
#include "run_fwd.h"
#include <thread>

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
		_req.data = (void*)this;
	}

	task(const func& method, const post_type& post_method)
		: _method(method)
		, _post_method(post_method)
		, _has_post(true)
	{
		_req.data = (void*)this;
	}

	~task() { get(); }

	task(const task&) = delete;
	task& operator=(const task&) = delete;

	R& get()
	{
		if (_has_post && _post_method)
		{
			return _result_post.get_future()->get();
		}
		else
		{
			return _result.get_future()->get();
		}
	}

	void execute()
	{
		std::cout << "-- execute start -- " << std::this_thread::get_id() << std::endl;
		try
		{
			_result.set_value(_method());
		}
		catch (...)
		{
			try
			{
				_result.set_exception(std::current_exception());
			}
			catch (...)
			{
				;
			}
		}
		_result.signal();

		if (_has_post && _post_method)
		{
			auto post_task = std::bind(_post_method, std::cref(_result.get_future()->get()));
			try
			{
				_result_post.set_value( post_task() );
			}
			catch (...)
			{
				try
				{
					_result_post.set_exception(std::current_exception());
				}
				catch (...)
				{
					;
				}
			}
			_result_post.signal();
		}
		std::cout << "-- execute end -- " << std::this_thread::get_id() << std::endl;
	}

	uv_work_t* get_raw_req() { return &_req; }

protected:
	func _method;
	promise<R> _result;
	promise<R> _result_post;
	post_type _post_method;
	bool _has_post;
	uv_work_t _req;
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
        , _result()
        , _result_post()
        , _post_method()
		, _has_post(false)
	{
		_req.data = (void*)this;
	}

	task(const func& method, const post_type& post_method)
		: _method(method)
        , _result()
        , _result_post()
		, _post_method(post_method)
		, _has_post(true)
	{
		_req.data = (void*)this;
	}

	~task() { get(); }

	task(const task& te) = delete;
	task& operator=(const task& te) = delete;

	void get()
	{
		if (_has_post && _post_method)
		{
			_result_post.get_future()->get();
		}
		else
		{
			_result.get_future()->get();
		}
	}

	void execute()
	{
		try
		{
			_method();
		}
		catch (...)
		{
			try
			{
				_result.set_exception(std::current_exception());
			}
			catch (...)
			{
				;
			}
		}
		_result.signal();

		if (_has_post && _post_method)
		{
			_result.get_future()->get();
			auto post_task = std::bind(_post_method);
			try
			{
				post_task();
			}
			catch (...)
			{
				try
				{
					_result_post.set_exception(std::current_exception());
				}
				catch (...)
				{
					;
				}
			}
			_result_post.signal();
		}
	}

	uv_work_t* get_raw_req() { return &_req; }

protected:
	func _method;
	promise<void> _result;
	promise<void> _result_post;
	post_type _post_method;
	bool _has_post;
	uv_work_t _req;
};

}

#endif

