#ifndef _RUN_H_
#define _RUN_H_

#include <memory>
#include <uv.h>
#include "run_fwd.h"
#include "task.h"
#include <thread>

namespace asyncply {

template <typename Function>
void __do(const shared_task<Function>& job)
{
	auto loop = uv_default_loop();
	uv_queue_work(loop, job->get_raw_req(),
		[](uv_work_t *req)
		{
			auto crossjob = (task_of_functor<Function>*)req->data;
			auto pending = [&crossjob](){
				std::cout << "-- start -- " << std::this_thread::get_id() << std::endl;
				crossjob->execute();
				std::cout << "-- end -- " << std::this_thread::get_id() << std::endl;
			};
			pending();
		},
		[](uv_work_t *req, int status)
		{
			std::cout << "status = " << status << std::endl;
		}
	);
	uv_run(loop, UV_RUN_DEFAULT);
}

template <typename Function>
shared_task<Function> run(Function&& f)
{
	auto job = std::make_shared< task_of_functor<Function> >(std::forward<Function>(f));
	__do<Function>(job);
	return job;
}

template <typename Function, typename FunctionPost>
shared_task<Function> run(Function&& f, FunctionPost&& fp)
{
	auto job = std::make_shared< task_of_functor<Function> >(std::forward<Function>(f), std::forward<FunctionPost>(fp));
	__do<Function>(job);
	return job;
}

}

#endif

