#include "run.h"

namespace asyncply {

static uv_loop_t* loop;
// static bool interrupted = false;
// static std::thread j;

init_asyncply::init_asyncply()
{
	loop = uv_default_loop();
}

init_asyncply::~init_asyncply()
{
	uv_loop_close(loop);
}

uv_loop_t* get_loop()
{
	return loop;
}

asyncply_API void __run()
{
	uv_run(loop, UV_RUN_NOWAIT);
	// uv_run(loop, UV_RUN_DEFAULT);
	// uv_run(loop, UV_RUN_ONCE);
}

asyncply_API void __stop()
{

}

asyncply_API void start()
{
	// j = std::move(std::thread([]() {
	// 			while(!interrupted)
	// 			{
	// 				asyncply::__run();
	// 			}
	// 		}));
}

asyncply_API void stop()
{
	// interrupted = true;
	// j.join();
	// asyncply::__stop();
}

}

