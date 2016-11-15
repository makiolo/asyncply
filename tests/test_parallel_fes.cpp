#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <atomic>
#include <assert.h>
#include <fes/async_fast.h>
#include "../parallel.h"
#include "../task.h"
#include "../algorithm.h"
#include <gtest/gtest.h>

class ParallelFesTest : testing::Test
{

};

TEST(ParallelFesTest, Test1)
{
	std::atomic<int> total;
	{
		total = 0;
		fes::async_fast<int> channel;
		channel.connect([&total](int data) {
			total += data;
		});
		asyncply::parallel_sync(
			[&]()
			{
				// consumer
				for(int i=0; i<10; ++i)
					channel.get();
			},
			[&]()
			{
				// consumer
				for(int i=0; i<10; ++i)
					channel.get();
			},
			[&]()
			{
				// consumer
				for(int i=0; i<10; ++i)
					channel.get();
			},
			[&]()
			{
				// producer
				std::vector<int> v;
				for(int i=0; i<10; ++i)
				{
					v.emplace_back(i*2);
				}
				asyncply::for_each_sync(v.begin(), v.end(), [&](int data){
					channel(data);
				});
			},
			[&]()
			{
				// producer
				std::vector<int> v;
				for(int i=0; i<10; ++i)
				{
					v.emplace_back(i*3);
				}
				asyncply::for_each_sync(v.begin(), v.end(), [&](int data){
					channel(data);
				});
			},
			[&]()
			{
				// producer
				std::vector<int> v;
				for(int i=0; i<10; ++i)
				{
					v.emplace_back(i*4);
				}
				asyncply::for_each_sync(v.begin(), v.end(), [&](int data){
					channel(data);
				});
			});
	}
	ASSERT_EQ(total, 405);
}
