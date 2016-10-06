#include "run.h"

namespace asyncply {

	template <typename Iterator, typename Function>
	void for_each_sync(Iterator a, Iterator b, Function&& f)
	{
		std::vector<task_t<void> > vf;
		for (; a != b; ++a) {
			auto desref = *a;
			vf.emplace_back(asyncply::run([&f, desref]() {
				f(desref);
			}));
		}
		for(auto& v : vf)
		{
			v->get();
		}
	}

	template <typename Iterator, typename Function>
	auto for_each(Iterator a, Iterator b, Function&& f)
	{
		return asyncply::run(
				[a, b, &f]()
				{
					for_each_sync(a, b, std::forward<Function>(f));
				});
	}

}

