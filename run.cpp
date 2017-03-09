#include "run.h"
// 

namespace asyncply {

asyncply::thread_pool& getThreadPool(void)
{
    static asyncply::thread_pool defaultPool( std::thread::hardware_concurrency() - 1 );
    return defaultPool;
}

}
