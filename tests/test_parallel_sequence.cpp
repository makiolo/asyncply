#include <iostream>
#include <vector>
#include <cmath>
#include <gtest/gtest.h>
#include "../parallel.h"
#include "../sequence.h"
#include "../task.h"

class ParallelSequenceTest : testing::Test
{

};

TEST(ParallelSequenceTest, Test1)
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
			double partial = job->get()->get();
			aggregation += partial;
		}
		catch (std::exception& e)
		{
			std::cout << "exception: " << e.what() << std::endl;
		}
	}
	ASSERT_EQ(aggregation, 32.0);
}
