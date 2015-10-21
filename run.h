#ifndef _RUN_H_
#define _RUN_H_

#include <memory>
#include <Poco/ThreadPool.h>
#include <multithread/multithread.h>
//#include "allocator.h"
#include "run_fwd.h"
#include "task.h"

namespace asyncply {

static asyncply::pool_thread g_pool;
static bool g_init = g_pool.run();

template <typename Function>
shared_task<Function> run(Function&& f)
{
	//static Allocator<task_of_functor<Function> > alloc;

	//auto job = std::allocate_shared< task_of_functor<Function> >(alloc, std::forward<Function>(f));
	auto job = std::make_shared< task_of_functor<Function> >(std::forward<Function>(f));
	auto& job_ref = *job;
	g_pool.submit(job_ref);
	return job;
}

template <typename Function, typename FunctionPost>
shared_task<Function> run(Function&& f, FunctionPost&& fp)
{
	//static Allocator<task_of_functor<Function> > alloc;

	//auto job = std::allocate_shared<task_of_functor<Function> >(alloc, std::forward<Function>(f), std::forward<FunctionPost>(fp));
	auto job = std::make_shared< task_of_functor<Function> >(std::forward<Function>(f), std::forward<FunctionPost>(fp));
	auto& job_ref = *job;
	g_pool.submit(job_ref);
	return job;
}

}

#endif

