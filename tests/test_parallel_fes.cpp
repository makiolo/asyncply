#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <atomic>
#include <assert.h>
#include <fes/sync.h>
#include <fes/async_fast.h>
#include <fes/async_delay.h>
#include "../parallel.h"
#include "../task.h"
#include "../algorithm.h"
#include <gtest/gtest.h>

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

class ParallelFesTest : testing::Test
{

};

TEST(ParallelFesTest, Test1)
{
	std::atomic<int> total;
	{
		total = 0;
		fes::async_fast<int> channel;
		channel.connect([&total](int data) {
			total += data;
		});
		asyncply::parallel_sync(
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
				std::vector<int> v;
				for(int i=0; i<10; ++i)
				{
					v.emplace_back(i*2);
				}
				asyncply::for_each_sync(v.begin(), v.end(), [&](int data){
					channel(data);
				});
			},
			[&]()
			{
				// producer
				std::vector<int> v;
				for(int i=0; i<10; ++i)
				{
					v.emplace_back(i*3);
				}
				asyncply::for_each_sync(v.begin(), v.end(), [&](int data){
					channel(data);
				});
			},
			[&]()
			{
				// producer
				std::vector<int> v;
				for(int i=0; i<10; ++i)
				{
					v.emplace_back(i*4);
				}
				asyncply::for_each_sync(v.begin(), v.end(), [&](int data){
					channel(data);
				});
			});
	}
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

TEST(ParallelFesTest, TestAsyncDelay)
{
	fes::async_delay<foo> channel;
	channel.connect([](const foo& f) {
		std::cout << "<async_delay> received foo" << std::endl;
	});
	// send foo
	channel(foo("bar"));
	// recv foo
	channel.get();
}
