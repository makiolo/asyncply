#include <iostream>
#include <vector>
#include <cmath>
#include <assert.h>
#include <atomic>
#include <algorithm>
#include "../algorithm.h"
#include "../parallel.h"
#include "../sequence.h"
#include "../async.h"
#include "../task.h"

int main(int, const char**)
{
	std::future<int> f = asyncply::_async([](){return 10;});
	std::cout << "f = " << f.get() << std::endl;

	auto f2 = asyncply::async([](){return 15;});
	f2->then([](int data){ std::cout << "post, received: " << data << std::endl; return 0; });
	std::cout << "f2 = " << f2->get() << std::endl;

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
		// std::cout << "thread " << std::this_thread::get_id() << std::endl;
	});
	std::cout << "total = " << total << std::endl;

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
		std::cout << "process1 = " << total_ps << std::endl;
		if (std::abs(6.0 - total_ps) > 1e-3)
		{
			std::cout << "invalid total " << total_ps << std::endl;
			return 1;
		}
	}

	{
		auto process2 = asyncply::parallel(
					[]()
					{
						std::cout << "hi" << std::endl;
					},
					[]()
					{
						std::cout << "bye" << std::endl;
					}
				);
		process2->then([]()
				{
					std::cout << "no accum" << std::endl;
				});
	}

	return 0;
}

