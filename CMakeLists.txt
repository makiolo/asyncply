project(asyncply CXX)
cmake_minimum_required(VERSION 3.0)
include("$ENV{CMAKI_PWD}/node_modules/npm-mas-mas/cmaki/cmaki.cmake")
cmaki_find_package(boost-headers)
cmaki_find_package(boost-coroutine2)
cmaki_find_package(teelogging)
cmaki_library(asyncply run.cpp PTHREADS)
add_subdirectory(tests)

