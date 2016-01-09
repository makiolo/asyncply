#ifndef _TASK_H_
#define _TASK_H_

#include <functional>
#include <uv.h>
#include "run_fwd.h"
#include "future.h"
#include <thread>
#include <uv.h>

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
		uv_loop_t* loop = get_loop();
		_req.data = (void*)this;
		uv_queue_work(loop, get_raw_req(),
				[](uv_work_t *req)
				{
					auto crossjob = (task<R>*)req->data;
					crossjob->execute();
				},
				NULL
			);
	}

	task(const func& method, const post_type& post_method)
		: _method(method)
		, _post_method(post_method)
		, _has_post(true)
	{
		uv_loop_t* loop = get_loop();
		_req.data = (void*)this;
		uv_queue_work(loop, get_raw_req(),
				[](uv_work_t *req)
				{
					auto crossjob = (task<R>*)req->data;
					crossjob->execute();
				},
				[](uv_work_t *req, int status)
				{
					auto crossjob = (task<R>*)req->data;
					crossjob->execute_post(status);
				}
			);
	}

	~task() { ; }

	task(const task&) = delete;
	task& operator=(const task&) = delete;

	R& await()
	{
		if(has_post())
		{
			return _result_post.await();
		}
		else
		{
			return _result.await();
		}
	}

	void execute()
	{
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
	}

	void execute_post(int status)
	{
		try
		{
			if (status == UV_ECANCELED)
			{
				std::cout << "task cancel exception" << std::endl;
				throw std::exception();
			}
			else
			{
				_result_post.set_value( _post_method( _result.await() ) );
			}
		}
		catch(...)
		{
			try
			{
				_result_post.set_exception( std::current_exception() );
			}
			catch (...)
			{
				;
			}
		}
		_result_post.signal();
	}

	inline bool has_post() const { return (_has_post && _post_method); }
	inline uv_work_t* get_raw_req() { return &_req; }

	/*
	 * only can be canceled, if its still in the queue
	 */
	inline void cancel()
	{
		uv_cancel( (uv_req_t*)get_raw_req() );
	}

protected:
	func _method;
	future<R> _result;
	future<R> _result_post;
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
		uv_loop_t* loop = get_loop();
		_req.data = (void*)this;
		uv_queue_work(loop, get_raw_req(),
				[](uv_work_t *req)
				{
					auto crossjob = (task<void>*)req->data;
					crossjob->execute();
				},
				NULL
			);
	}

	task(const func& method, const post_type& post_method)
		: _method(method)
        , _result()
        , _result_post()
		, _post_method(post_method)
		, _has_post(true)
	{
		uv_loop_t* loop = get_loop();
		_req.data = (void*)this;
		uv_queue_work(loop, get_raw_req(),
				[](uv_work_t *req)
				{
					auto crossjob = (task<void>*)req->data;
					crossjob->execute();
				},
				[](uv_work_t *req, int status)
				{
					auto crossjob = (task<void>*)req->data;
					crossjob->execute_post(status);
				}
			);
	}

	~task() { ; }

	task(const task& te) = delete;
	task& operator=(const task& te) = delete;

	void await()
	{
		if(has_post())
		{
			_result_post.await();
		}
		else
		{
			_result.await();
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
	}

	void execute_post(int status)
	{
		try
		{
			_result.await();
			if (status == UV_ECANCELED)
			{
				std::cout << "task cancel exception" << std::endl;
				throw std::exception();
			}
			else
			{
				auto post_task = std::bind(_post_method);
				post_task();
			}
		}
		catch(...)
		{
			try
			{
				_result_post.set_exception( std::current_exception() );
			}
			catch (...)
			{
				;
			}
		}
		_result_post.signal();
	}

	inline bool has_post() const { return (_has_post && _post_method); }
	inline uv_work_t* get_raw_req() { return &_req; }

	/*
	 * only can be canceled, if its still in the queue
	 */
	inline void cancel()
	{
		uv_cancel( (uv_req_t*)get_raw_req() );
	}

protected:
	func _method;
	future<void> _result;
	future<void> _result_post;
	post_type _post_method;
	bool _has_post;
	uv_work_t _req;
};

}

#endif

