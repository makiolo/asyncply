#ifndef _ASYNCPLY_ALGORITHM_H_
#define _ASYNCPLY_ALGORITHM_H_

#include "run.h"

namespace asyncply {

	// read: http://www.andythomason.com/2016/08/21/c-multithreading-an-effective-parallel-for-loop/
	
	template <typename Iterator, typename Function>
	void for_each(Iterator a, Iterator b, Function&& f)
	{
		std::vector<shared_task<void> > vf;
		for (; a != b; ++a) {
			auto& desref = *a;
			vf.emplace_back(asyncply::async([f = std::forward<Function>(f), desref = std::forward<decltype(desref)>(desref)]() {
				f(desref);
			}));
		}
		for(auto& v : vf)
			v->get();
	}

	template <typename Iterator, typename Function>
	auto for_each_async(Iterator a, Iterator b, Function&& f)
	{
		return asyncply::async(
				[a, b, &f]()
				{
					asyncply::for_each(a, b, std::forward<Function>(f));
				});
	}

}

#endif

