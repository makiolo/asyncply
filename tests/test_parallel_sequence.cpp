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
	double total = asyncply::parallel_sync(
		[&]()
		{
			return asyncply::sequence_sync(7.0,
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
			return asyncply::sequence_sync(9.0,
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

