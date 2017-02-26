#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <atomic>
#include <assert.h>
#include <gtest/gtest.h>
#include <fast-event-system/sync.h>
#include <fast-event-system/async_fast.h>
#include <fast-event-system/async_delay.h>
#include "../parallel_async.h"
#include "../task.h"
#include "../algorithm.h"

class foo
{
public:
	explicit foo() { std::cout << "constructor empty" << std::endl; }
	explicit foo(const std::string& data) : _data(data) { std::cout << "constructor with parms" << std::endl; }
	virtual ~foo() { std::cout << "destructor" << std::endl; }

	foo(const foo& other) : _data(other._data) { std::cout << "constructor copia" << std::endl; }
	foo& operator=(const foo& other)
	{
		std::cout << "operador asignación" << std::endl;
		foo(other).swap(*this);
		return *this;
	}

	foo(foo&& other) : _data(std::move(other._data)) { std::cout << "constructor de movimiento" << std::endl; }

	foo& operator=(foo&& other)
	{
		std::cout << "operador asignacion de movimiento" << std::endl;
		foo(std::move(other)).swap(*this);
		return *this;
	}

	void swap(foo& other)
	{
		std::cout << "swap" << std::endl;
		using std::swap;
		swap(_data, other._data);
	}

private:
	std::string _data;
};

class ParallelFesTest : testing::Test { };

TEST(ParallelFesTest, Test1)
{
	std::atomic<int> total;
	total = 0;
	fes::async_fast<int> channel;
	channel.connect([&total](int data) {
		total += data;
	});
	asyncply::parallel(
		[&]()
		{
			// consumer
			for(int i=0; i<10; ++i)
				channel.get();
		},
		[&]()
		{
			// consumer
			for(int i=0; i<10; ++i)
				channel.get();
		},
		[&]()
		{
			// consumer
			for(int i=0; i<10; ++i)
				channel.get();
		},
		[&]()
		{
			// producer
			for(int i=0; i<10; ++i)
			{
				channel(i*2);
			}
		},
		[&]()
		{
			// producer
			for(int i=0; i<10; ++i)
			{
				channel(i*3);
			}
		},
		[&]()
		{
			// producer
			for(int i=0; i<10; ++i)
			{
				channel(i*4);
			}
		});
	ASSERT_EQ(total, 405);
}

TEST(ParallelFesTest, TestSync)
{
	fes::sync<foo> channel;
	channel.connect([](const foo& f) {
		std::cout << "<sync> received foo" << std::endl;
	});
	// send foo
	channel(foo("bar"));
}

TEST(ParallelFesTest, TestAsyncFast)
{
	fes::async_fast<foo> channel;
	channel.connect([](const foo& f) {
		std::cout << "<async_fast> received foo" << std::endl;
	});
	// send foo
	channel(foo("bar"));
	// recv foo
	channel.get();
}

// TEST(ParallelFesTest, DISABLED_TestAsyncDelay)
// {
// 	fes::async_delay<foo> channel;
// 	channel.connect([](auto& f) {
// 		std::cout << "<async_delay> received foo" << std::endl;
// 	});
// 	// send foo
// 	channel(foo("bar"));
// 	// recv foo
// 	channel.get();
// }

