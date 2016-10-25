#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <atomic>
#include <assert.h>
#include <async_fast.h>
#include "../parallel.h"
#include "../task.h"
#include "../algorithm.h"

int main(int, const char**)
{
	try
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
		std::cout << "total " << total << std::endl;
		if (total != 405)
		{
			std::cout << "invalid total " << total << std::endl;
			throw std::exception();
		}
	}
	catch (std::exception& e)
	{
		std::cout << "general exception " << e.what() << std::endl;
		return 1;
	}

	return 0;
}

