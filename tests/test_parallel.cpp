#include <iostream>
#include <vector>
#include <cmath>
#include <assert.h>
#include <gtest/gtest.h>
#include "../parallel.h"
#include "../task.h"

class ParallelTest : testing::Test { ; };

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
