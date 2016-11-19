#include <iostream>
#include <vector>
#include <cmath>
#include <assert.h>
#include <asyncply/parallel.h>
#include <asyncply/task.h>
#include <gtest/gtest.h>

class ParallelTest : testing::Test { };

TEST(ParallelTest, Test1)
{
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
	ASSERT_EQ(task_parallel->get(), 32.0);
}

TEST(ParallelTest, Test2)
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
	ASSERT_EQ(aggregation, 32.0);
}
