#include "run.h"

namespace asyncply {

	template <typename Iterator, typename Function>
	void for_each(Iterator a, Iterator b, Function&& f)
	{
		std::vector<task_t<void> > vf;
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

