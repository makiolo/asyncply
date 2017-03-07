#include <iostream>
#include <vector>
#include <cmath>
#include <gtest/gtest.h>
#include "../parallel_async.h"
#include "../sequence_async.h"
#include "../task.h"

class ParallelSequenceTest : testing::Test { ; };

TEST(ParallelSequenceTest, test_sequence_and_parallel)
{
	double total = asyncply::parallel(
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
	ASSERT_EQ(total, 32.0);
}

