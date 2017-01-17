project(asyncply CXX)
cmake_minimum_required(VERSION 3.2.2)

set(CMAKI_REPOSITORY "http://artifacts.myftp.biz:8080")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmaki/cmaki.cmake")
cmaki_setup()

cmaki_find_package(fast-event-system)
include_directories(thread-pool-cpp/thread_pool)
cmaki2_library(asyncply run.cpp PTHREADS)

add_subdirectory(tests)
