#include <iostream>
#include <complex>
#include <cmath>
#include <thread>
#include <vector>
#include <gtest/gtest.h>
#include "../sequence.h"
#include "../parallel.h"
#include "../run.h"

class SequenceTest : testing::Test { };

TEST(SequenceTest, test_sequence_sync)
{
	struct control_flow
	{
		bool code;

		operator bool() const
		{
			return code;
		}
	};
	control_flow flow_out = asyncply::sequence(control_flow{true},
		[](control_flow flow) {
			std::cout << "code 1" << std::endl;
			return flow;
		},
		[](control_flow flow) {
			std::cout << "code 2" << std::endl;
			return flow;
		},
		[](control_flow flow) {
			std::cout << "code 3" << std::endl;
			flow.code = false;
			return flow;
		},
		[](control_flow flow) {
			std::cout << "code 4" << std::endl;
			return flow;
		}
	);
	ASSERT_FALSE(flow_out.code);
}

TEST(SequenceTest, test_sequence_async)
{
	struct control_flow
	{
		bool code;

		operator bool() const
		{
			return code;
		}
	};
	auto task = asyncply::sequence_async(control_flow{true},
		[](control_flow flow) {
			std::cout << "code 1" << std::endl;
			return flow;
		},
		[](control_flow flow) {
			std::cout << "code 2" << std::endl;
			return flow;
		},
		[](control_flow flow) {
			std::cout << "code 3" << std::endl;
			flow.code = false;
			return flow;
		},
		[](control_flow flow) {
			std::cout << "code 4" << std::endl;
			return flow;
		}
	);
	control_flow flow_out = task->get();
	ASSERT_FALSE(flow_out.code);
}

