#ifndef _RUN_H_
#define _RUN_H_

#include <memory>
#include "run_fwd.h"
#include "task.h"

#include <uvpp/loop.hpp>
#include <uvpp/timer.hpp>
#include <uvpp/async.hpp>
#include <uvpp/work.hpp>
#include <uvpp/tcp.hpp>
#include <uvpp/idle.hpp>
#include <uvpp/resolver.hpp>

namespace asyncply {

template <typename Function>
shared_task<Function> run(Function&& f)
{
	auto job = std::make_shared< task_of_functor<Function> >(std::forward<Function>(f));
	auto loop = uv_default_loop();
	auto empty_fb = [](uv_work_t *req, int status) {};
	job->req.data = (void*)(&(*job));
	uv_queue_work(loop, &(job->req), [](uv_work_t *req) {
		auto crossjob = (task_of_functor<Function>*)req->data;
		auto pending = [&crossjob](){
			crossjob->execute();
		};
		pending();
	}, empty_fb);
	uv_run(loop, UV_RUN_DEFAULT);
	return job;
}

template <typename Function, typename FunctionPost>
shared_task<Function> run(Function&& f, FunctionPost&& fp)
{
	auto job = std::make_shared< task_of_functor<Function> >(std::forward<Function>(f), std::forward<FunctionPost>(fp));
	auto loop = uv_default_loop();
	auto empty_fb = [](uv_work_t *req, int status) {};
	job->req.data = (void*)(&(*job));
	uv_queue_work(loop, &(job->req), [](uv_work_t *req) {
		auto crossjob = (task_of_functor<Function>*)req->data;
		auto pending = [&crossjob](){
			crossjob->execute();
		};
		pending();
	}, empty_fb);
	uv_run(loop, UV_RUN_DEFAULT);
	return job;
}

}

#endif

