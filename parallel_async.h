#ifndef _PARALLEL_H_
#define _PARALLEL_H_

#include <vector>
#include <numeric>
#include "metacommon/common.h"
#include "run.h"

namespace asyncply {

template <typename Function>
auto wtf_return_type_is_this()
{
	return [](){
		using ret_t = typename std::result_of<Function()>::type::element_type::return_type;
		return ret_t();
	};
}

template <typename Function>
void _parallel(std::vector<shared_task<Function> >& vf, Function&& f)
{
	vf.emplace_back(asyncply::async(std::forward<Function>(f)));
}

template <typename Function, typename... Functions>
void _parallel(std::vector<shared_task<Function> >& vf, Function&& f, Functions&&... fs)
{
	vf.emplace_back(asyncply::async(std::forward<Function>(f)));
	asyncply::_parallel(vf, std::forward<Functions>(fs)...);
}

/////////////////////////////////////////////////////////

template <  typename Function,
			typename Container,
			class = typename std::enable_if<
				(!std::is_arithmetic<typename std::result_of<Function()>::type>::value) &&
				(!std::is_same<typename std::result_of<Function()>::type, bool>::value) &&
				(!std::is_same<typename std::result_of<Function()>::type, task_t< double > >::value)
			>::type
	>
std::vector<typename std::result_of<Function()>::type> aggregation(Container&& vf)
{
	using ret_t = typename std::result_of<Function()>::type;
	std::vector<ret_t> results;
	for(auto& v : vf)
		results.emplace_back(v->get());
	return results;
}

template <  typename Function,
			typename Container,
			class = typename std::enable_if<
				(std::is_arithmetic<typename std::result_of<Function()>::type>::value) &&
				(!std::is_same<typename std::result_of<Function()>::type, bool>::value) &&
				(!std::is_same<typename std::result_of<Function()>::type, task_t< double > >::value)
			>::type
	>
typename std::result_of<Function()>::type aggregation(Container&& vf)
{
	using ret_t = typename std::result_of<Function()>::type;
	std::vector<ret_t> results;
	for(auto& v : vf)
		results.emplace_back(v->get());
	return std::accumulate(results.begin(), results.end(), ret_t(0), std::plus<ret_t>());
}

template <  typename Function,
			typename Container,
			class = typename std::enable_if<
				(!std::is_arithmetic<typename std::result_of<Function()>::type>::value) &&
				(std::is_same<typename std::result_of<Function()>::type, bool>::value) &&
				(!std::is_same<typename std::result_of<Function()>::type, task_t< double > >::value)
			>::type
	>
bool aggregation(Container&& vf)
{
	using ret_t = typename std::result_of<Function()>::type;
	std::vector<ret_t> results;
	for(auto& v : vf)
		results.emplace_back(v->get());
	return std::accumulate(results.begin(), results.end(), true, std::logical_and<bool>());
}

template <  typename Function,
			typename Container,
			class = typename std::enable_if<
				(!std::is_arithmetic<typename std::result_of<Function()>::type>::value) &&
				(!std::is_same<typename std::result_of<Function()>::type, bool>::value) &&
				(std::is_same<typename std::result_of<Function()>::type, task_t< double > >::value)
			>::type
	>
auto aggregation(Container&& vf)
{
	using ret0_t = typename std::result_of<Function()>::type;
	std::vector<ret0_t> results0;
	for(auto& v : vf)
		results0.emplace_back(v->get());
	return aggregation<decltype(wtf_return_type_is_this<Function>())>(results0);
}

////////////////////////////////////////////////////////

template <  typename Function,
			typename ... Functions,
			class = typename std::enable_if<
				(!std::is_void<typename std::result_of<Function()>::type>::value)
			>::type
	>
auto _parallel_sync(shared_task<Function>& task, Function&& f_, Functions&&... fs_)
{
	assert(task == nullptr);
	task = asyncply::async(
		std::bind(
			[f = std::move(f_)](auto&& ... fs)
			{
				std::vector<shared_task<Function> > vf;
				asyncply::_parallel(vf, f, fs...);
				return aggregation<Function>(vf);
			}, std::forward<Functions>(fs_)...
		)
	);
	return task;
}

template <  typename Function,
			typename ... Functions,
			class = typename std::enable_if<
				(std::is_void<typename std::result_of<Function()>::type>::value)
			>::type
	>
void _parallel_sync(shared_task<Function>& task, Function&& f_, Functions&&... fs_)
{
	assert(task == nullptr);
	task = asyncply::async(
		std::bind(
			[f = std::move(f_)](auto&& ... fs)
			{
				std::vector<shared_task<Function> > vf;
				asyncply::_parallel(vf, f, fs...);
			}, std::forward<Functions>(fs_)...
		)
	);
}

template <  typename Function,
			typename ... Functions,
			class = typename std::enable_if<
				(!std::is_void<typename std::result_of<Function()>::type>::value)
			>::type
	>
auto parallel(Function&& f, Functions&&... fs)
{
	shared_task<Function> task;
	asyncply::_parallel_sync(task, std::forward<Function>(f), std::forward<Functions>(fs)...);
	return task->get();
}

template <  typename Function,
			typename ... Functions,
			class = typename std::enable_if<
				(std::is_void<typename std::result_of<Function()>::type>::value)
			>::type
	>
void parallel(Function&& f, Functions&&... fs)
{
	shared_task<Function> task;
	asyncply::_parallel_sync(task, std::forward<Function>(f), std::forward<Functions>(fs)...);
	task->get();
}

template <typename Function, typename... Functions>
auto parallel_async(Function&& f, Functions&&... fs)
{
	shared_task<Function> task;
	asyncply::_parallel_sync(task, std::forward<Function>(f), std::forward<Functions>(fs)...);
	return task;
}

}

#endif

