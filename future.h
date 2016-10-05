#ifndef _FUTURE_H_
#define _FUTURE_H_

#include <atomic>
#include <exception>
// #include <Poco/Semaphore.h>

namespace asyncply {

// class Semaphore
// {
// public:
// 	Semaphore(int value, int max)
// 	{
//
// 	}
// 	void set()
// 	{
// 		
// 	}
// 	
// 	void wait()
// 	{
// 		
// 	}
// };
//
// template <typename R>
// class future
// {
// public:
// 	future()
// 		: _ready(false)
// 		, _has_exception(false)
// 		, _semaphore(0, 1)
// 		, _value()
// 	{
// 	}
//
// 	~future()
// 	{
// 		;
// 	}
//
// 	future(const future&) = delete;
// 	future& operator=(const future&) = delete;
//
// 	R& get()
// 	{
// 		if(!_ready)
// 		{
// 			_semaphore.wait();
// 			_ready = true;
// 			if (_has_exception && _exception)
// 			{
// 				std::rethrow_exception(_exception);
// 			}
// 		}
// 		return _value;
// 	}
//
// 	void signal() { _semaphore.set(); }
// 	void set_value(const R& value) { _value = value; }
// 	void set_value(R&& value) { _value = std::forward<R>(value); }
// 	void set_exception(std::exception_ptr p)
// 	{
// 		_exception = p;
// 		_has_exception = true;
// 	}
//
// protected:
// 	std::atomic<bool> _ready;
// 	std::atomic<bool> _has_exception;
// 	std::exception_ptr _exception;
// 	asyncply::Semaphore _semaphore;
// 	// atomic ?
// 	R _value;
// };
//
// template <>
// class future<void>
// {
// public:
// 	future()
// 		: _exception()
// 		, _ready(false)
// 		, _semaphore(0, 1)
// 	{
// 	}
//
// 	~future()
// 	{
// 		;
// 	}
//
// 	future(const future&) = delete;
// 	future& operator=(const future&) = delete;
//
// 	void get()
// 	{
// 		if(!_ready)
// 		{
// 			_semaphore.wait();
// 			_ready = true;
// 			if (_exception)
// 			{
// 				std::rethrow_exception(_exception);
// 			}
// 		}
// 	}
//
// 	void signal() { _semaphore.set(); }
// 	void set_exception(std::exception_ptr p) { _exception = p; }
//
// protected:
// 	std::exception_ptr _exception;
// 	std::atomic<bool> _ready;
// 	asyncply::Semaphore _semaphore;
// };

}

#endif

