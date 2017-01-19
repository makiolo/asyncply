#ifndef _ASYNC_H_
#define _ASYNC_H_

#include "run_fwd.h"

namespace asyncply {

template <typename Function, typename ... Args>
future_of_functor<Function, Args...> _async(Function&& f, Args&& ... args)
{
	return __async(std::forward<Function>(f), std::forward<Args>(args)...);
}

}

#endif
