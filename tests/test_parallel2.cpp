#include <iostream>
#include <vector>
#include <cmath>
#include <assert.h>
#include <atomic>
#include <algorithm>
#include "../algorithm.h"
#include "../parallel.h"
#include "../sequence.h"
#include "../task.h"

int main(int, const char**)
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
	std::cout << "total = " << total << std::endl;

	{
		auto process1 = asyncply::parallel(
					[]()
					{
						return asyncply::sequence_sync(1.0,
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
						return asyncply::sequence_sync(1.0,
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
		process1->then([](int accum)
				{
					assert(accum == 6);
					std::cout << "accum is " << accum << std::endl;
					return 0;
				});
		assert(process1->get() == 0);
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

