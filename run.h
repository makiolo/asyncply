#ifndef _RUN_H_
#define _RUN_H_

#include <thread>
#include <iostream>
#include <memory>
#include <uv.h>
#include <uv-threadpool.h>
#include "run_fwd.h"
#include "task.h"

namespace asyncply {

struct asyncply_API init_asyncply
{
	init_asyncply();
	~init_asyncply();
};
asyncply_API init_asyncply _init_asyncply;

struct pool
{
	pool()
	{
		asyncply::start();
	}
	~pool()
	{
		asyncply::stop();
	}
};
static pool _p;

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

