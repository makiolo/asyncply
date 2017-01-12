#ifndef _RUN_H_
#define _RUN_H_

#include <thread>
#include <iostream>
#include <memory>
#include "run_fwd.h"
#include "task.h"

namespace asyncply {

struct asyncply_API register_asyncply
{
	register_asyncply();
	~register_asyncply();
};
asyncply_API register_asyncply _reg;

template <typename Function, typename ... Args>
shared_task<Function, Args...> async(Function&& f, Args&& ... args)
{
	return std::make_shared< task_of_functor<Function, Args...> >(std::forward<Function>(f), std::forward<Args>(args)...);
}

}

#endif
