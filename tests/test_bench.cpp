#include <iostream>
#include <memory>
#include <ctime>
#include <chrono>
#include <sstream>
#include <fstream>
#include <cmath>
#include <atomic>
#include <algorithm>
#include "../algorithm.h"
#include "../sequence.h"
#include "../parallel.h"
#include <gtest/gtest.h>

class BenchTest : testing::Test { };

struct measure_scoped
{
	// TODO: decision by compiler
	// use best clock for you
	using clock = std::chrono::high_resolution_clock;
	// using clock = std::chrono::steady_clock;
	// using clock = std::chrono::system_clock;

	using result_t = double;
	using time_point_t = std::chrono::time_point<clock>;
	using duration_t = std::chrono::duration<result_t>;

	measure_scoped(result_t& result)
		: _start(clock::now())
		, _result(result)
	{
		;
	}

	~measure_scoped()
	{
		time_point_t end = clock::now();
		duration_t elapsedtime = std::chrono::duration_cast<duration_t>(end - _start);
		_result = elapsedtime.count();
	}

protected:
	time_point_t _start;
	result_t& _result;
};

int main_measured_algorithm_1()
{
	std::vector<int> a;
	for(int i=0; i<200; ++i)
	{
		a.push_back(1);
		a.push_back(4);
		a.push_back(12);
		a.push_back(-3);
		a.push_back(22);
	}
	std::atomic<int> total;
	total = 0;
	asyncply::for_each_sync(a.begin(), a.end(), [&total](int i) {
		total += i;
	});
	return 0;
}

int main_measured_algorithm_2()
{
	std::vector<int> a;
	for(int i=0; i<200; ++i)
	{
		a.push_back(1);
		a.push_back(4);
		a.push_back(12);
		a.push_back(-3);
		a.push_back(22);
	}
	std::atomic<int> total;
	total = 0;
	std::for_each(a.begin(), a.end(), [&total](int i) {
		total += i;
	});
	return 0;
}

double launch_benchmark(int (*algorithm)())
{
	long long N = 1e2;
	double elapsedtime;
	{
		measure_scoped timer(elapsedtime);

		for (int i = 0; i < N; ++i)
			volatile int result = algorithm();
	}
	return (elapsedtime * 1e9) / double(N);  // return mean time
}

TEST(BenchTest, TestAsyncply)
{
	double t1 = launch_benchmark(main_measured_algorithm_1);
	std::cout << "t1 = " << t1 << " ns" << std::endl;
}

TEST(BenchTest, TestStd)
{
	double t2 = launch_benchmark(main_measured_algorithm_2);
	std::cout << "t2 = " << t2 << " ns" << std::endl;
}
