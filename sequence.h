#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include <functional>
#include "run.h"

namespace asyncply {

template <typename Data, typename Function>
void __sequence(task_t<Data>& task, Data data, Function&& f)
{
	if(!task)
	{
		task = asyncply::async( 	
			[](Data d, Function&& f)
			{
				return f(d);
			},
			data, std::forward<Function>(f)
		);
	}
	else
	{
		task = task->then(
			[](Data d, Function&& f)
			{
				return f(d);
			},
			std::placeholders::_1, std::forward<Function>(f)
		);
	}
}
	
template <typename Data, typename Function>
void _sequence(task_t<Data>& task, Data data, Function&& f)
{
	__sequence(task, data, std::forward<Function>(f));
}

template <typename Data, typename Function, typename... Functions>
void _sequence(task_t<Data>& task, Data data, Function&& f, Functions&& ... fs)
{
	__sequence(task, data, std::forward<Function>(f));
	asyncply::_sequence<Data>(task, data, std::forward<Functions>(fs)...);
}

template <typename Data, typename... Functions>
Data sequence_sync(Data data, Functions&&... fs)
{
	task_t<Data> task;
	asyncply::_sequence<Data>(task, data, std::forward<Functions>(fs)...);
	return task->get();
}

template <typename Data, typename... Functions>
task_t<Data> sequence(Data data, Functions&&... fs)
{
	return asyncply::async(
			[](Data d, Functions&&... fs)
			{
				return asyncply::sequence_sync(d, std::forward<Functions>(fs)...);
			},
			data, std::forward<Functions>(fs)...
	);
}

}

#endif
