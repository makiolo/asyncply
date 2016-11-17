#include <atomic>
#include <asyncply/cmd.h>
#include <asyncply/parallel.h>
#include <asyncply/run.h>
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
	std::vector<asyncply::coroptr<void> > coros;
	for(int i=1; i<10; ++i)
	{
		coros.emplace_back(asyncply::corun<void>(
			[=](asyncply::yield_type<void>& yield)
			{
				std::cout << "create " << i << std::endl;
				yield();
				std::cout << "download " << i << std::endl;
				yield();
				std::cout << "patching " << i << std::endl;
				yield();
				std::cout << "compile " << i << std::endl;
				yield();
				std::cout << "tests " << i << std::endl;
				yield();
				std::cout << "packing " << i << std::endl;
				yield();
				std::cout << "destroy " << i << std::endl;
			}
		));
	}

	bool any_updated = true;
	while(any_updated)
	{
		any_updated = false;
		for(auto& c : coros)
		{
			if(*c)
			{
				(*c)();
				any_updated = true;
			}
		}
	}
}

