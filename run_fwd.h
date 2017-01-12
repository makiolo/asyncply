#ifndef _RUN_FWD_H_
#define _RUN_FWD_H_

#include <memory>
#include <type_traits>
#include <future>
#include "api.h"
#include "task_fwd.h"

namespace asyncply {

// functor_type
template <typename Function, typename ... Args>
using return_of_functor = typename std::result_of<Function(Args...)>::type;

// task_functor
template <typename Function, typename ... Args>
using task_of_functor = asyncply::task< return_of_functor<Function, Args...> >;

// future_functor
template <typename Function>
using future_of_functor = std::future< return_of_functor<Function> >;
  
// shared_task
template <typename Type>
using task_t = std::shared_ptr< asyncply::task<Type> >;

// shared_task_functor
template <typename Function, typename ... Args>
using shared_task = std::shared_ptr<task_of_functor<Function, Args...> >;
  
template <typename Function, typename ... Args>
shared_task<Function, Args...> async(Function&& f, Args&& ... args);

}

#endif

