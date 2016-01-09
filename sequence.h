#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include <functional>
#include "run.h"

namespace asyncply {

template <typename Data, typename Function>
std::function<Data(Data)> _sequence(Function&& f)
{
	return [&](Data data)
	{
		auto job = asyncply::run(
			[&]()
			{
				return f(data);
			});
		return Data(job->await());
	};
}

template <typename Data, typename Function, typename... Functions>
std::function<Data(Data)> _sequence(Function&& f, Functions&&... fs)
{
	return [&](Data data)
	{
		auto job = asyncply::run(
			[&]()
			{
				return f(data);
			},
#ifdef _WIN32
			[&](Data d)
#else
			// solution for bug in gcc 4.9.3
			[&fs...](Data d)
#endif
			{
				return asyncply::_sequence<Data>(std::forward<Functions>(fs)...)(d);
			});
		return Data(job->await());
	};
}

template <typename Data, typename... Functions>
task_t<Data> sequence(Data data, Functions&&... fs)
{
	auto task = asyncply::run(
			[data, &fs...]()
			{
				return asyncply::_sequence<Data>(std::forward<Functions>(fs)...)(data);
			});
	return task;
}

}

#endif

