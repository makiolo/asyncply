PROJECT(ASYNCPLY CXX)
cmake_minimum_required(VERSION 2.8)
add_subdirectory(verypoco)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmaki/cmaki.cmake")
cmaki_setup()

# ===============================================================
include(ExternalProject)
set_property(DIRECTORY PROPERTY EP_STEP_TARGETS configure build test)
ExternalProject_Add (
	project_libuv
	GIT_REPOSITORY "https://github.com/libuv/libuv.git"
	GIT_TAG "v1.x"
	PREFIX "${CMAKE_SOURCE_DIR}/depends/libuv"
	CONFIGURE_COMMAND "${CMAKE_SOURCE_DIR}/depends/libuv/src/project_libuv/autogen.sh"
	COMMAND sh -c "${CMAKE_SOURCE_DIR}/depends/libuv/src/project_libuv/configure --prefix=${CMAKE_SOURCE_DIR}/libuv"
	BUILD_COMMAND sh -c "make -j8"
	INSTALL_COMMAND sh -c "make -j8 install"
	BUILD_IN_SOURCE 1
)
include_directories(${CMAKE_SOURCE_DIR}/libuv/include)
link_directories(${CMAKE_SOURCE_DIR}/libuv/lib)
# ===============================================================

add_subdirectory(multithread)
include_directories(metacommon)
cmaki_library(asyncply run.cpp DEPENDS uv multithread verypoco PTHREADS)
add_dependencies(asyncply project_libuv)
add_subdirectory(tests)

