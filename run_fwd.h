#ifndef _RUN_FWD_H_
#define _RUN_FWD_H_

#include <memory>
#include <type_traits>
#include "api.h"
#include "task_fwd.h"

namespace ctpl {
  class thread_pool;
}

namespace asyncply {

// forward
// template <typename T> class TaskFuture;
class ThreadPool;

// thread pool
// using thread_pool = ThreadPool;
using thread_pool = ctpl::thread_pool;

// functor_type
template <typename Function, typename ... Args>
using result_type = decltype(std::declval<Function>()(std::declval<Args>()...));

// task_functor
template <typename Function, typename ... Args>
using task_of_functor = asyncply::task< result_type<Function, Args...> >;
  
// future_functor
template <typename Function, typename ... Args>
using future_of_functor = std::future< result_type<Function, Args...> >;

// shared_task
template <typename Type>
using task_t = std::shared_ptr< asyncply::task<Type> >;

// shared_task_functor
template <typename Function, typename ... Args>
using shared_task = std::shared_ptr< task_of_functor<Function, Args...> >;

// __async
template <typename Function, typename ... Args>
auto __async(Function&& f, Args&& ... data) -> future_of_functor<Function, Args...>;

// async
template <typename Function, typename ... Args>
auto async(Function&& f, Args&& ... args) -> shared_task<Function, Args...>;

// when_any

// when_all

}

#endif
