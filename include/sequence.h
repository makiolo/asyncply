#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include <functional>
#include "run.h"

namespace asyncply {

template <typename Data, typename Function>
void __sequence(shared_task<Data>& task, Data data, Function&& f)
{
	if(!task)
	{
		task = asyncply::async( 	
			[f = std::move(f)](Data d) -> Data
			{
				return f(d);
			}, std::forward<Data>(data)
		);
	}
	else
	{
		task = task->then(
			std::bind(
				[f = std::move(f)](Data d) -> Data
				{
					return f(d);
				}, std::placeholders::_1
			)
		);
	}
}
	
template <typename Data, typename Function>
void _sequence(shared_task<Data>& task, Data data, Function&& f)
{
	asyncply::__sequence(task, data, std::forward<Function>(f));
}

template <typename Data, typename Function, typename... Functions>
void _sequence(shared_task<Data>& task, Data data, Function&& f, Functions&& ... fs)
{
	asyncply::__sequence(task, data, std::forward<Function>(f));
	asyncply::_sequence(task, std::forward<Data>(data), std::forward<Functions>(fs)...);
}

template <typename Data, typename... Functions>
auto sequence(Data data, Functions&&... fs)
{
	shared_task<Data> task;
	asyncply::_sequence(task, std::forward<Data>(data), std::forward<Functions>(fs)...);
	return task->get();
}

template <typename Data, typename... Functions>
auto asequence(Data data, Functions&&... fs)
{
	shared_task<Data> task;
	asyncply::_sequence(task, std::forward<Data>(data), std::forward<Functions>(fs)...);
	return task;
}

}

#endif
