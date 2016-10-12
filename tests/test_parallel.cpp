#include <iostream>
#include <vector>
#include <cmath>
#include <assert.h>
#include "../parallel.h"
#include "../task.h"

int main(int, const char**)
{
	try
	{
		for (int i = 0; i < 100; ++i)
		{
			std::vector< asyncply::task_t<double> > vjobs;
			asyncply::_parallel(vjobs,
				[]()
				{
					return 9.0;
				},
				[]()
				{
					return 7.0;
				},
				[]()
				{
					return 10.0;
				},
				[]()
				{
					return 6.0;
				});
			double aggregation = 0.0;
			for (auto& job : vjobs)
			{
				try
				{
					aggregation += job->get();
				}
				catch (std::exception& e)
				{
					std::cout << "exception: " << e.what() << std::endl;
					throw;
				}
			}
			if (std::abs(aggregation - 32.0) > 1e-3)
			{
				std::cout << "invalid total " << aggregation << std::endl;
				return 1;
			}

			auto task_parallel = asyncply::parallel(
				[]()
				{
					return 9.0;
				},
				[]()
				{
					return 7.0;
				},
				[]()
				{
					return 10.0;
				},
				[]()
				{
					return 6.0;
				});
			double total = task_parallel->get();
			if (std::abs(aggregation - total) > 1e-3)
			{
				std::cout << "invalid total " << total << std::endl;
				return 1;
			}
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

