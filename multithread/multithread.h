#ifndef _MULTITHREAD_H_
#define _MULTITHREAD_H_

#include <iostream>
#include <functional>
#include <iostream>
#include <algorithm>
#include <assert.h>
#if defined(__unix__) || defined(__APPLE__) || defined(__GNUC__)
#include <semaphore.h>
#include <pthread.h>
#else
#include <process.h>
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif
#endif

#ifndef JOINABLE
#define JOINABLE
#endif

#include "api.h"

namespace asyncply
{
	class worker;
	class thread;
	class synchronizer;
	class semaphore;
	class pool_thread;
	template <typename T> class circular_queue;
	class mutex;
	class job;
	class job_function;
	template <typename T> class scoped_lock;
}

#include "circular_queue.h"
#include "pool_thread.h"
#include "worker.h"
#include "sem.h"
#include "synchronizer.h"
#include "thread.h"
#include "mutex.h"
#include "job.h"
#include "job_functor.h"

#endif // _MULTITHREAD_H_

