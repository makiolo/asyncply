#include "run.h"
// 

namespace asyncply {

asyncply::thread_pool& getThreadPool(void)
{
    static asyncply::thread_pool defaultPool;
    return defaultPool;
}

}
