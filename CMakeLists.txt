PROJECT(ASYNCPLY CXX)
cmake_minimum_required(VERSION 2.8)

set(CMAKI_REPOSITORY "http://artifacts.myftp.biz:8080")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmaki/cmaki.cmake")
cmaki_setup()

cmaki_find_package(fast-event-system)
include_directories(metacommon)
include_directories(thread-pool-cpp/thread_pool)
include_directories(stackless_coroutine)
include_directories(fast-event-system)
cmaki_library(asyncply run.cpp PTHREADS)
add_subdirectory(tests)


