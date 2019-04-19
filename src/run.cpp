#include "run.h"

#if defined(__clang__)
#define STATIC_MULTITHREAD static
#elif defined(__GNUC__) || defined(__GNUG__)
#define STATIC_MULTITHREAD static __thread
#elif defined(_MSC_VER)
#define STATIC_MULTITHREAD __declspec(thread) static
#endif

namespace asyncply {

asyncply::thread_pool& getThreadPool(void)
{
    STATIC_MULTITHREAD asyncply::thread_pool defaultPool( std::thread::hardware_concurrency() - 1 );
    return defaultPool;
}

}
