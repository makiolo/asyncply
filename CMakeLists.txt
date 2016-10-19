PROJECT(ASYNCPLY CXX)
cmake_minimum_required(VERSION 2.8)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmaki/cmaki.cmake")
cmaki_setup()

include_directories(metacommon)
include_directories(thread-pool-cpp/thread_pool)
include_directories(stackless_coroutine)
cmaki_library(asyncply run.cpp PTHREADS)
add_subdirectory(tests)

