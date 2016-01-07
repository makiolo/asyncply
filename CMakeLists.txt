PROJECT(ASYNCPLY CXX)
cmake_minimum_required(VERSION 2.8)
add_subdirectory(verypoco)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmaki/utils.cmake")
ENABLE_MODERN_CPP()

include(ExternalProject)
set_property(DIRECTORY PROPERTY EP_STEP_TARGETS configure build test)
ExternalProject_Add (
	project_libuv
	GIT_REPOSITORY "https://github.com/libuv/libuv.git"
	GIT_TAG "master"
	PREFIX "${CMAKE_SOURCE_DIR}/depends/libuv"
	CONFIGURE_COMMAND "${CMAKE_SOURCE_DIR}/depends/libuv/src/project_libuv/autogen.sh"
	COMMAND sh -c "${CMAKE_SOURCE_DIR}/depends/libuv/src/project_libuv/configure --prefix=${CMAKE_SOURCE_DIR}/libuv"
	BUILD_COMMAND sh -c "make -j8"
	INSTALL_COMMAND sh -c "make -j8 install"
	BUILD_IN_SOURCE 1
)

include_directories(metacommon)
add_subdirectory(multithread)
add_subdirectory(tests)

