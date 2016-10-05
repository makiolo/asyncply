#ifndef _RUN_H_
#define _RUN_H_

#include <thread>
#include <iostream>
#include <memory>
#include "run_fwd.h"
#include "task.h"
//
#include <thread_pool.hpp>

namespace asyncply {

static ThreadPool pool;

struct asyncply_API register_asyncply
{
	register_asyncply();
	~register_asyncply();
};
asyncply_API register_asyncply _reg;

template <typename Function>
shared_task<Function> run(Function&& f)
{
	return std::make_shared< task_of_functor<Function> >(std::forward<Function>(f));
}

template <typename Function, typename FunctionPost>
shared_task<Function> run(Function&& f, FunctionPost&& fp)
{
	return std::make_shared< task_of_functor<Function> >(std::forward<Function>(f), std::forward<FunctionPost>(fp));
}

}

#endif

