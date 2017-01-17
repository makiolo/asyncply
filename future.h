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
	
	inline bool valid() const
	{
		return m_future.valid();
	}

	inline void wait() const
	{
		m_future.wait();
	}

	template< class Rep, class Period >
	inline std::future_status wait_for( const std::chrono::duration<Rep,Period>& timeout_duration ) const
	{
		return m_future.wait_for(timeout_duration);
	}

	template< class Clock, class Duration >
	inline std::future_status wait_until( const std::chrono::time_point<Clock,Duration>& timeout_time ) const
	{
		return m_future.wait_until(timeout_time);
	}

private:
	std::future<T> m_future;
};

}

#endif
