#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include <functional>
#include "run.h"

namespace asyncply {

template <typename Data, typename Function>
std::function<Data(const Data&)> _sequence(Function&& f)
{
#ifdef _WIN32
	return [&](const Data& data)
#else
	return [&f](const Data& data)
#endif
	{
		auto job = asyncply::run(
			[&]()
			{
				return f(data);
			});
		return job->get();
	};
}

template <typename Data, typename Function, typename... Functions>
std::function<Data(const Data&)> _sequence(Function&& f, Functions&&... fs)
{
#ifdef _WIN32
	return [&](const Data& data)
#else
	return [&f, &fs...](const Data& data)
#endif
	{
		auto job = asyncply::run(
#ifdef _WIN32
			[&]()
#else
			[&data, &f]()
#endif
			{
				return f(data);
			},
#ifdef _WIN32
			[&](const Data& d)
#else
			[&fs...](const Data& d)
#endif
			{
				return asyncply::_sequence<Data>(std::forward<Functions>(fs)...)(d);
			});
		return job->get();
	};
}

template <typename Data, typename... Functions>
Data sequence(const Data& data, Functions&&... fs)
{
	auto job = asyncply::run(
			[&]()
			{
				return asyncply::_sequence<Data>(std::forward<Functions>(fs)...)(data);
			});
	return job->get();
}

}

#endif

