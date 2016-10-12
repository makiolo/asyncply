#ifndef _PARALLEL_H_
#define _PARALLEL_H_

#include <vector>
#include <numeric>
#include "common.h"
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
				(!std::is_same<typename std::result_of<Function()>::type, future_of_functor<Function> >::value)
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
				(!std::is_same<typename std::result_of<Function()>::type, future_of_functor<Function> >::value)
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
				(!std::is_same<typename std::result_of<Function()>::type, future_of_functor<Function> >::value)
			>::type
	>
bool aggregation(Container&& vf)
{
	using ret_t = bool;
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
				(std::is_same<typename std::result_of<Function()>::type, future_of_functor<Function> >::value)
			>::type
	>
auto aggregation(Container&& vf)
{
	using ret_t = typename std::result_of<Function()>::type;
	std::vector<ret_t*> results;
	for(auto& v : vf)
		results.emplace_back(&(v->get()));
	return aggregation<Function, const std::vector<ret_t*>& >(results);
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

template <  typename Function,
			typename ... Functions
			// class = typename std::enable_if<
			// 	(!std::is_void<typename std::result_of<Function()>::type>::value)
			// >::type
	>
auto parallel(Function&& f, Functions&&... fs)
{
	return asyncply::async(
			[&f, &fs...]()
			{
				return asyncply::parallel_sync(std::forward<Function>(f), std::forward<Functions>(fs)...);
			});
}

// template <  typename Function,
// 			typename ... Functions,
// 			class = typename std::enable_if<
// 				(std::is_void<typename std::result_of<Function()>::type>::value)
// 			>::type
// 	>
// auto parallel(Function&& f, Functions&&... fs)
// {
// 	return asyncply::async(
// 			[&f, &fs...]()
// 			{
// 				asyncply::parallel_sync(std::forward<Function>(f), std::forward<Functions>(fs)...);
// 			});
// }

template <  typename Function
			// class = typename std::enable_if<
			// 	(!std::is_void<typename std::result_of<Function()>::type>::value)
			// >::type
	>
auto parallel(Function&& f)
{
	return asyncply::async(
			[&f]()
			{
				return asyncply::parallel_sync(std::forward<Function>(f));
			});
}

// template <  typename Function,
// 			class = typename std::enable_if<
// 				(std::is_void<typename std::result_of<Function()>::type>::value)
// 			>::type
// 	>
// auto parallel(Function&& f)
// {
// 	return asyncply::async(
// 			[&f]()
// 			{
// 				asyncply::parallel_sync(std::forward<Function>(f));
// 			});
// }

}

#endif

