#include <iostream>
#include <complex>
#include <cmath>
#include "../sequence.h"
#include "../parallel.h"
#include "../run.h"
#include <thread>
#include <vector>

int main(int, const char**)
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
	auto task = asyncply::sequence(flow,
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
	task->get();

	std::cout << bool(23) << std::endl;
	std::cout << bool(0) << std::endl;
	std::cout << bool("hola") << std::endl;
	std::cout << bool("") << std::endl;
	std::cout << flow << std::endl;
	control_flow flow2 = {false};
	std::cout << flow2 << std::endl;
	return 0;
}

