#ifndef _ASYNC_H_
#define _ASYNC_H_

#include "run_fwd.h"
#include <thread_pool.hpp>

namespace asyncply {

static ThreadPool __pool;

template <typename Function>
void async(future_of_functor<Function>& result, Function&& f)
{
	std::packaged_task<return_of_functor<Function>()> t(f);
	result = t.get_future();
	__pool.post(t);
}

}

#endif

