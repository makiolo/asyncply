#include <iostream>
#include <vector>
#include <cmath>
#include "../parallel.h"
#include "../sequence.h"
#include "../task.h"

asyncply::pool p;

int main(int, const char**)
{
	try
	{
		for (int i = 0; i < 100; ++i)
		{
			std::vector<asyncply::task_t< asyncply::task_t<double> > > vjobs;
			asyncply::_parallel(vjobs,
				[&]()
				{
					return asyncply::sequence(7.0,
						[](double data)
						{
							return data + 3.0;
						},
						[](double data)
						{
							return data + 4.0;
						});
				},
				[&]()
				{
					return asyncply::sequence(9.0,
						[](double data)
						{
							return data + 5.0;
						},
						[](double data)
						{
							return data + 4.0;
						});
				});
			double aggregation = 0.0;
			for (auto& job : vjobs)
			{
				try
				{
					// double await await
					double partial = job->await()->await();
					aggregation += partial;
				}
				catch (std::exception& e)
				{
					std::cout << "exception: " << e.what() << std::endl;
				}
			}
			if (std::abs(aggregation - 32.0) > 1e-3)
			{
				std::cout << "invalid total " << aggregation << std::endl;
				throw std::exception();
			}
			std::cout << "aggregation = " << aggregation << std::endl;
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
