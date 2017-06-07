#include "run.h"
// 

#ifdef _WIN32
#define STATIC_MULTITHREAD __declspec(thread) static
#else
#define STATIC_MULTITHREAD static __thread
#endif

namespace asyncply {

asyncply::thread_pool& getThreadPool(void)
{
    STATIC_MULTITHREAD asyncply::thread_pool defaultPool( std::thread::hardware_concurrency() - 1 );
    return defaultPool;
}

}
