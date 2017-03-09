#include <iostream>
#include <vector>
#include <cmath>
#include <assert.h>
#include <gtest/gtest.h>
#include "../parallel_async.h"
#include "../task.h"

class ParallelTest : testing::Test { ; };

#if 0

TEST(ParallelTest, test_parallel)
{
	auto result = asyncply::parallel(
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
	ASSERT_EQ(result, 32.0);
}

#endif
