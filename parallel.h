#ifndef _PARALLEL_H_
#define _PARALLEL_H_

#include <vector>
#include <numeric>
#include <metacommon/common.h>
#include "run.h"

namespace asyncply {

template <typename Function>
void _parallel(std::vector<shared_task<Function>>& vf, Function&& f)
{
	vf.emplace_back(asyncply::async(std::forward<Function>(f)));
}

template <typename Function, typename... Functions>
void _parallel(std::vector<shared_task<Function>>& vf, Function&& f, Functions&&... fs)
{
	vf.emplace_back(asyncply::async(std::forward<Function>(f)));
	asyncply::_parallel(vf, std::forward<Functions>(fs)...);
}

//

template <  typename Function,
			typename Container,
			class = typename std::enable_if<
				(!std::is_arithmetic<typename std::result_of<Function()>::type>::value) &&
				(!std::is_same<typename std::result_of<Function()>::type, bool>::value) &&
				(!std::is_same<typename std::result_of<Function()>::type, task_t<double> >::value)
			>::type
	>
std::vector<typename std::result_of<Function()>::type> aggregation(Container&& vf)
{
	using ret_t = typename std::result_of<Function()>::type;
	std::vector<ret_t> results;
	for(auto& v : vf)
		results.emplace_back(v->get());
	return std::move(results);
}

template <  typename Function,
			typename Container,
			class = typename std::enable_if<
				(std::is_arithmetic<typename std::result_of<Function()>::type>::value) &&
				(!std::is_same<typename std::result_of<Function()>::type, bool>::value) &&
				(!std::is_same<typename std::result_of<Function()>::type, task_t<double> >::value)
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
				(!std::is_same<typename std::result_of<Function()>::type, task_t<double> >::value)
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
				(std::is_same<typename std::result_of<Function()>::type, task_t<double> >::value)
			>::type
	>
auto aggregation(Container&& vf)
{
	using ret0_t = typename std::result_of<Function()>::type;
	std::vector<ret0_t> results0;
	for(auto& v : vf)
		results0.emplace_back(v->get());

	auto f = [](){
		using ret_t = typename std::result_of<Function()>::type::element_type::return_type;
		return ret_t();
	};
	// static_assert(std::is_same<decltype(f), std::function<double()> >::value, "error in f");
	return aggregation<decltype(f)>(results0);
}

//

template <  typename Function,
			typename ... Functions,
			class = typename std::enable_if<
				(!std::is_void<typename std::result_of<Function()>::type>::value)
			>::type
	>
auto parallel_sync(Function&& f, Functions&&... fs)
{
	std::vector<shared_task<Function>> vf;
	vf.emplace_back(asyncply::async(std::forward<Function>(f)));
	asyncply::_parallel(vf, std::forward<Functions>(fs)...);
	return aggregation<Function, const std::vector<shared_task<Function> >& >(vf);
}

template <  typename Function,
			typename ... Functions,
			class = typename std::enable_if<
				(std::is_void<typename std::result_of<Function()>::type>::value)
			>::type
	>
void parallel_sync(Function&& f, Functions&&... fs)
{
	std::vector<shared_task<Function> > vf;
	vf.emplace_back(asyncply::async(std::forward<Function>(f)));
	asyncply::_parallel(vf, std::forward<Functions>(fs)...);
	for(auto& v : vf)
	{
		v->get();
	}
}

//

template <typename Function, typename ... Functions>
auto parallel(Function&& f, Functions&&... fs)
{
	return asyncply::async(
		[](Function&& f, Functions&&... fs)
		{
			return asyncply::parallel_sync(std::forward<Function>(f), std::forward<Functions>(fs)...);
		},
		std::forward<Function>(f), std::forward<Functions>(fs)...
	);
}

template <typename Function>
auto parallel(Function&& f)
{
	return asyncply::async(
		[](Function&& f)
		{
			return asyncply::parallel_sync(std::forward<Function>(f));
		},
		std::forward<Function>(f)
	);
}

}

#endif

