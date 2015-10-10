#include <multithread/api.h>
#include <multithread/MultiThreading.h>
#include <multithread/Mutex.h>

namespace asyncply {

mutex::mutex()
#if defined(LINUX)
	: _mutex()
	, _attr()
#endif
{
#if defined(LINUX)
	if (pthread_mutexattr_init(&_attr) != 0)
	{
		throw std::runtime_error("error initializing mutex");
	}
	pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_NORMAL);
	pthread_mutex_init(&_mutex, &_attr);
#elif defined(__APPLE__)

#else
	// iniciar mutex / seccion critica
	InitializeCriticalSection(&_section_critical);
#endif
}

mutex::~mutex()
{
#if defined(LINUX)
	pthread_mutexattr_destroy(&_attr);
	pthread_mutex_destroy(&_mutex);
#elif defined(__APPLE__)

#else
	DeleteCriticalSection(&_section_critical);
#endif
}

}

