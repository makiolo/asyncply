#ifndef _RUN_FWD_H_
#define _RUN_FWD_H_

#include <memory>
#include <type_traits>
#include "api.h"
#include "task_fwd.h"

namespace asyncply {

// forward
template <typename T> class TaskFuture;
class ThreadPool;

// functor_type
template <typename Function>
using return_of_functor = typename std::result_of<Function()>::type;

// task_functor
template <typename Function>
using task_of_functor = asyncply::task< return_of_functor<Function> >;

// future_functor
template <typename Function>
using future_of_functor = TaskFuture< return_of_functor<Function> >;

// shared_task
template <typename Type>
using task_t = std::shared_ptr< asyncply::task<Type> >;

// shared_task_functor
template <typename Function>
using shared_task = std::shared_ptr< task_of_functor<Function> >;

// async
template <typename Function, typename ... Args>
shared_task<Function> async(Function&& f, Args&& ... args);

template <typename Function, typename... Args>
inline auto submitJob(Function&& func, Args&&... args);

ThreadPool& getThreadPool(void);

// when_any

// when_all

}

#endif
