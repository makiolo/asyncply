#include <iostream>
#include <vector>
#include <cmath>
#include <assert.h>
#include <atomic>
#include <algorithm>
#include <gtest/gtest.h>
#include "../algorithm.h"
#include "../parallel_async.h"
#include "../sequence_async.h"
#include "../async.h"
#include "../task.h"

class Parallel2Test : testing::Test { };

TEST(Parallel2Test, Test1)
{
	auto f1 = asyncply::_async([](int data){return data;}, 10);
	ASSERT_EQ(f1.get(), 10);
}

TEST(Parallel2Test, Test2)
{
	auto f1 = asyncply::async([](){return 15;});
	auto f2 = f1->then([](int data) {
		std::cout << "post, received: " << data << std::endl;
		return data + 6;
	});
	ASSERT_EQ(f2->get(), 15 + 6);
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
	asyncply::for_each(a.begin(), a.end(), [&total](int i) {
		total += i;
	});
	ASSERT_EQ(total, 3600);
}

TEST(Parallel2Test, Test3_async)
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
	auto task = asyncply::for_each_async(a.begin(), a.end(), [&total](int i) {
		total += i;
	});
	task->get();
	ASSERT_EQ(total, 3600);
}

#if 0

TEST(Parallel2Test, collapse_double)
{
	double total_ps = asyncply::parallel(
		[]()
		{
			double data = 1.0;
			return asyncply::sequence(data,
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
			double data = 1.0;
			return asyncply::sequence(data,
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

#endif

#if 1

TEST(Parallel2Test, Test5)
{
	std::atomic<int> total;
	total = 0;
	auto process = asyncply::parallel_async(
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
	auto process2 = process->then([&total]()
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
		auto process = asyncply::parallel_async(
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
		auto process_then = process->then([&total]()
				{
					std::cout << "no accum" << std::endl;
					total += 1;
				});
		process_then->get();
	}
	ASSERT_EQ(total, 3);
}

#endif

