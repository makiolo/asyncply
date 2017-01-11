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
		auto job = asyncply::async(
			[&]()
			{
				return f(data);
			});
		return Data(job->get());
	};
}

template <typename Data, typename Function, typename... Functions>
std::function<Data(Data)> _sequence(Function&& f, Functions&&... fs)
{
	return [&](Data data)
	{
		auto job = asyncply::async( 	[&]()
						{
							return f(data);
						}
					  );
		auto then_job = job->then(
#ifdef _MSC_VER
			[&](Data d)
#else
			// solution for bug in gcc 4.9.3
			[&fs...](Data d)
#endif
			{
				return asyncply::_sequence<Data>(std::forward<Functions>(fs)...)(d);
			}
		);
		
		return Data(then_job->get());
	};
}

template <typename Data, typename... Functions>
Data sequence_sync(Data data, Functions&&... fs)
{
	return asyncply::_sequence<Data>(std::forward<Functions>(fs)...)(data);
}

template <typename Data, typename... Functions>
auto sequence(Data data, Functions&&... fs)
{
	return asyncply::async(
			[data, &fs...]()
			{
				return sequence_sync<Data>(data, std::forward<Functions>(fs)...);
			});
}

}

#endif

