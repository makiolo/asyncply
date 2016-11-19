#include <iostream>
#include <vector>
#include <cmath>
#include <assert.h>
#include <atomic>
#include <algorithm>
#include <asyncply/algorithm.h>
#include <asyncply/parallel.h>
#include <asyncply/sequence.h>
#include <asyncply/async.h>
#include <asyncply/task.h>
#include <gtest/gtest.h>

class Parallel2Test : testing::Test { };

TEST(Parallel2Test, Test1)
{
	std::future<int> f = asyncply::_async([](){return 10;});
	ASSERT_EQ(f.get(), 10);
}

TEST(Parallel2Test, Test2)
{
	auto f2 = asyncply::async([](){return 15;});
	f2->then([](int data){ std::cout << "post, received: " << data << std::endl; return 6; });
	// 6 or 15 ?
	ASSERT_EQ(f2->get(), 6);
}

TEST(Parallel2Test, Test3)
{
	std::vector<int> a;
	for(int i=0; i<100; ++i)
	{
		a.push_back(1);
		a.push_back(4);
		a.push_back(12);
		a.push_back(-3);
		a.push_back(22);
	}
	std::atomic<int> total;
	total = 0;
	asyncply::for_each_sync(a.begin(), a.end(), [&total](int i) {
		total += i;
		std::cout << "thread " << std::this_thread::get_id() << std::endl;
	});
	ASSERT_EQ(total, 3600);
}

TEST(Parallel2Test, Test4)
{
	auto total_ps = asyncply::parallel_sync(
				[]()
				{
					return asyncply::sequence(1.0,
						[](double data)
						{
							return data + 1.0;
						},
						[](double data)
						{
							return data + 1.0;
						});
				},
				[]()
				{
					return asyncply::sequence(1.0,
						[](double data)
						{
							return data + 1.0;
						},
						[](double data)
						{
							return data + 1.0;
						});
				}
			);
	ASSERT_EQ(total_ps, 6);
}

TEST(Parallel2Test, Test5)
{
	std::atomic<int> total;
	total = 0;
	auto process2 = asyncply::parallel(
				[&total]()
				{
					std::cout << "hi" << std::endl;
					total += 1;
				},
				[&total]()
				{
					std::cout << "bye" << std::endl;
					total += 1;
				}
			);
	process2->then([&total]()
			{
				std::cout << "no accum" << std::endl;
				total += 1;
			});
	process2->get();
	ASSERT_EQ(total, 3);
}

TEST(Parallel2Test, Test6)
{
	std::atomic<int> total;
	total = 0;
	{
		auto process2 = asyncply::parallel(
					[&total]()
					{
						std::cout << "hi" << std::endl;
						total += 1;
					},
					[&total]()
					{
						std::cout << "bye" << std::endl;
						total += 1;
					}
				);
		process2->then([&total]()
				{
					std::cout << "no accum" << std::endl;
					total += 1;
				});
	}
	ASSERT_EQ(total, 3);
}

