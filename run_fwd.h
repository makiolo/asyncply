#ifndef _RUN_FWD_H_
#define _RUN_FWD_H_

#include <memory>
#include <type_traits>
#include "api.h"
#include "task_fwd.h"
#include <coroutine/coroutine.h>

namespace ctpl {
  class thread_pool;
}

namespace asyncply {

using thread_pool = ctpl::thread_pool;

template <typename Function, typename ... Args>
using functor_type = decltype(std::declval<Function>()(std::declval<Args>()...));

template <typename Function, typename ... Args>
using task_functor = asyncply::task< functor_type<Function, Args...> >;
  
template <typename Function, typename ... Args>
using future_functor = std::future< functor_type<Function, Args...> >;

template <typename Type>
using shared_task = std::shared_ptr< asyncply::task<Type> >;

template <typename Function, typename ... Args>
using shared_task_functor = std::shared_ptr< task_functor<Function, Args...> >;

template <typename Function, typename ... Args>
auto _async(Function&& f, Args&& ... data) -> future_functor<Function, Args...>;

template <typename Function, typename ... Args>
auto async(Function&& f, Args&& ... args) -> shared_task_functor<Function, Args...>;

template <typename Function, typename ... Args>
auto await(cu::yield_type& yield, Args&& ... args) -> functor_type<Function, Args...>;

// when_any

// when_all

}

#endif
