#ifndef _FUTURE_H_
#define _FUTURE_H_

#include <future>
 
namespace asyncply
{

template <typename T>
class TaskFuture
{
public:
    TaskFuture(std::future<T>&& future)
	    : m_future{std::move(future)}
    {
    }

    TaskFuture(const TaskFuture& rhs) = delete;
    TaskFuture& operator=(const TaskFuture& rhs) = delete;
    TaskFuture(TaskFuture&& other) = default;
    TaskFuture& operator=(TaskFuture&& other) = default;
    ~TaskFuture(void)
    {
      if(m_future.valid())
      {
          m_future.get();
      }
    }

    auto get(void)
    {
      return m_future.get();
    }


private:
    std::future<T> m_future;
};

}

#endif
