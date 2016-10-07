#ifndef _ASYNC_H_
#define _ASYNC_H_

#include "run_fwd.h"
#include <thread_pool.hpp>

namespace asyncply {

static ThreadPool __pool;

template <typename Function>
future_of_functor<Function> _async(Function&& f)
{
	return __pool.process(std::forward<Function>(f));
}

}

#endif

