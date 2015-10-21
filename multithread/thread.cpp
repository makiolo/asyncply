#include "api.h"
#include "multithread.h"
#include "thread.h"

namespace asyncply
{

#ifdef _WIN32

DWORD thread::HandleGlobalMyThread(LPVOID parms)
{
	thread* t = static_cast<thread*>(parms);
	t->execute();

	return TRUE;
}

#else

void* thread::HandleGlobalMyThread(void* parms)
{
	thread* t = static_cast<thread*>(parms);
	t->execute();

#ifdef JOINABLE  // Joinable
	pthread_exit(0);
#endif
}

#endif

}

