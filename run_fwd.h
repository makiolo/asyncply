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
template <typename Function, typename ... Args>
// using return_of_functor = typename std::result_of<Function(Args...)>::type;
using return_of_functor = decltype(std::declval<Function>()(std::declval<Args>()...));

template <typename Function, typename ... Args>
// using result_type = typename std::result_of< return_of_functor<Function, Args...>() >::type;
using result_type = return_of_functor<Function, Args...>;
  
// task_functor
template <typename Function, typename ... Args>
using task_of_functor = asyncply::task< result_type<Function, Args...> >;
  
// future_functor
template <typename Function, typename ... Args>
using future_of_functor = TaskFuture< result_type<Function, Args...> >;

// shared_task
template <typename Type>
using task_t = std::shared_ptr< asyncply::task<Type> >;

// shared_task_functor
template <typename Function, typename ... Args>
using shared_task = std::shared_ptr< task_of_functor<Function, Args...> >;
  
// __async
template <typename Function, typename ... Args>
future_of_functor<Function, Args...> __async(Function&& f, Args&& ... data);
  
// async
template <typename Function, typename ... Args>
auto async(Function&& f, Args&& ... args) -> shared_task<Function, Args...>;

ThreadPool& getThreadPool(void);

// when_any

// when_all

}

#endif
