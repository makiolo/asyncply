#ifndef _ASYNC_H_
#define _ASYNC_H_

#include "run_fwd.h"
#include <thread_pool.hpp>

namespace asyncply {

template <typename Function, typename ... Args>
future_of_functor<Function> _async(Function&& f, Args&& ... data)
{
#if 1
	static ThreadPool __pool;
	return __pool.process(std::forward<Function>(f), std::forward<Args>(data)...);
#else
	return std::async(std::launch::async, std::forward<Function>(f), std::forward<Args>(data)...);
#endif
}

}

#endif

