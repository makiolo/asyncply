PROJECT(ASYNCPLY CXX)
cmake_minimum_required(VERSION 2.8)
add_subdirectory(verypoco)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmaki/utils.cmake")
ENABLE_MODERN_CPP()
add_subdirectory(multithread)
add_subdirectory(tests)

