#include "run.h"
// 

namespace asyncply {

ThreadPool& getThreadPool(void)
{
    static ctpl::thread_pool otherPool;
    static ThreadPool defaultPool;
    return defaultPool;
}

}
