#include <atomic>
#include <asyncply/cmd.h>
#include <asyncply/parallel.h>
#include <asyncply/run.h>
#include <asyncply/algorithm.h>
#include <gtest/gtest.h>

class CoroTest : testing::Test { };

using namespace asyncply;

TEST(CoroTest, Test1)
{
	std::vector<std::string> lines;
	cmd(find("../.."), grep("test_"), out(lines));
	for (auto& line : lines)
		std::cout << line << std::endl;
}

TEST(CoroTest, Test2)
{
	cmd(find("../.."),
		grep(".*\\.cpp$|.*\\.h$"),
		cat(),
		grep("class|struct|typedef|using|void|int|double|float"),
		grep_v("enable_if|;|\"|\'"),
		split(" "),
		trim(),
		uniq(),
		join(" "),
		out());
}

TEST(CoroTest, Test3)
{
	std::vector<asyncply::coroutine<int> > coros;
	for(int i=1; i<10; ++i)
	{
		coros.emplace_back(asyncply::make_coroutine<int>(
			[=](auto& yield)
			{
				std::cout << "create " << i << std::endl;
				yield(0);
				std::cout << "download " << i << std::endl;
				yield(0);
				std::cout << "patching " << i << std::endl;
				yield(0);
				std::cout << "compile " << i << std::endl;
				yield(0);
				std::cout << "tests " << i << std::endl;
				yield(0);
				std::cout << "packing " << i << std::endl;
				yield(0);
				std::cout << "destroy " << i << std::endl;
			}
		));
	}
	
	std::cout << "-- ticking coroutines" << std::endl;

#if 0
	// std::atomic<bool> any_updated;
	// any_updated = true;
	bool any_updated = true;
	while(any_updated)
	{
		any_updated = false;
		// asyncply::for_each_sync(coros.begin(), coros.end(), [&any_updated](auto& c) {
		for(auto& c : coros) {
			if(*c)
			{
				any_updated = true;
				(*c)();
			}
		// });
		}
	}
#else
	for(auto& co : coros) {
		for(auto& state : *co) { }
	}
#endif
}
