#include <iostream>
#include <vector>
#include <atomic>
#include "../parallel.h"
#include "../task.h"
#include <gtest/gtest.h>

class ConsumerProducerTest : testing::Test { };

TEST(ConsumerProducerTest, Test1)
{
	const size_t N = 1e4;
	std::atomic<size_t> f;
	f = 0;
	asyncply::parallel_sync(
		[&]()
		{
			for (size_t i = 0; i < (N + 1); ++i)
			{
				f += i;
			}
		},
		[&]()
		{
			for (size_t i = 0; i < N; ++i)
			{
				f -= i;
			}
		});
	ASSERT_EQ(f, N);
}
