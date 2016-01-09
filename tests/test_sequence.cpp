#include <iostream>
#include <complex>
#include <cmath>
#include "../sequence.h"
#include "../parallel.h"
#include "../run.h"
#include <thread>
#include <vector>

#define SEQUENCE 1
#define PARALLEL 1

// asyncply::pool p;

int main(int, const char**)
{
#if PARALLEL
	std::vector< asyncply::task_t<double> > vjobs;
	asyncply::_parallel(vjobs,
			[]()
			{
				// std::cout << "-- thread parallel " << std::this_thread::get_id() << std::endl;
				return 9.0;
			},
			[]()
			{
				// std::cout << "-- thread parallel " << std::this_thread::get_id() << std::endl;
				return 7.0;
			},
			[]()
			{
				// std::cout << "-- thread parallel " << std::this_thread::get_id() << std::endl;
				return 10.0;
			},
			[]()
			{
				// std::cout << "-- thread parallel " << std::this_thread::get_id() << std::endl;
				return 6.0;
			});
#endif

#if SEQUENCE
	int input_data = 10;
	auto task = asyncply::sequence(input_data,
			[](int data) {
				// std::cout << "-- thread sequence " << std::this_thread::get_id() << std::endl;
				std::cout << "hello ... ---" << data << "---" << std::endl;
				return data + 59;
			},
			[](int data) {
				// std::cout << "-- thread sequence " << std::this_thread::get_id() << std::endl;
				std::cout << "world ... ---" << data << "---" << std::endl;
				return data + 59;
			}
		);
#endif

#if PARALLEL
	for(auto& job : vjobs)
	{
		std::cout << "result parallel = " << job->await() << std::endl;
	}
#endif

#if SEQUENCE
	std::cout << "result = " << task->await() << std::endl;
#endif

	std::cout << "start old test" << std::endl;
	try
	{
		for (int i = 0; i < 100; ++i)
		{
			double input_data2 = 1.0;
			auto task_seq = asyncply::sequence(input_data2,
				[](double data)
				{
					return data + 2.0;
				},
				[](double data)
				{
					return data + 3.0;
				},
				[](double data)
				{
					return data + 4.0;
				},
				[](double data)
				{
					return data + 5.0;
				},
				[](double data)
				{
					return data + 6.0;
				});
			double total = task_seq->await();
			if (std::abs(total - 21.0) > 1e-3)
			{
				std::cout << "not expected result" << std::endl;
				throw std::exception();
			}
			std::cout << "total: " << total << std::endl;
		}
		std::cout << "result ok" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << "general exception " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

