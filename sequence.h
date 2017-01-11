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
						[](Data d, Function&& f)
						{
							return f(d);
						},
						data, std::forward<Function>(f)
					  );
		return Data(job->get());
	};
}

template <typename Data, typename Function, typename... Functions>
std::function<Data(Data)> _sequence(Function&& f, Functions&&... fs)
{
	return [&](Data data)
	{
		auto job = asyncply::async( 	
						[](Data d, Function&& f)
						{
							return f(d);
						},
						data, std::forward<Function>(f)
					  );
		auto then_job = job->then(
			[](Data d, Functions&&... fs)
			{
				return asyncply::_sequence<Data>(std::forward<Functions>(fs)...)(d);
			},
			std::placeholders::_1, std::forward<Functions>(fs)...
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
task_t<Data> sequence(Data data, Functions&&... fs)
{
	return asyncply::async(
			[](Data d, Functions&&... fs)
			{
				return asyncply::sequence_sync<Data>(d, std::forward<Functions>(fs)...);
			},
			data, std::forward<Functions>(fs)...
	);
}

}

#endif
