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

TEST(SequenceTest, Test1)
{
	struct control_flow
	{
		bool code;

		operator bool() const
		{
			return code;
		}
	};
	control_flow flow = {true};
	control_flow flow_out = asyncply::sequence_sync(flow,
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
