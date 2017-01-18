#include "run.h"
// 

namespace asyncply {

ThreadPool& getThreadPool(void)
{
    static ThreadPool defaultPool;
    return defaultPool;
}

}
