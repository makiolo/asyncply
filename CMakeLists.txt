project(asyncply CXX)
cmake_minimum_required(VERSION 3.5)
include("$ENV{CMAKI_PWD}/node_modules/npm-mas-mas/cmaki/cmaki.cmake")
cmaki_setup()
cmaki_git_clone(
	PROJECT_NAME    readerwriterqueue
	GIT_URL         https://github.com/cameron314/readerwriterqueue
	GIT_BRANCH      master
	DIRECTORY		${CMAKE_SOURCE_DIR}
)
cmaki_find_package_boost()
include_directories(.)
include_directories(include)
cmaki_library(asyncply src/run.cpp PTHREADS)
cmaki_google_test(bench tests/test_bench.cpp DEPENDS asyncply PTHREADS)
cmaki_google_test(parallel tests/test_parallel.cpp DEPENDS asyncply PTHREADS)
cmaki_google_test(consumer_producer tests/test_consumer_producer.cpp DEPENDS asyncply PTHREADS)
cmaki_google_test(parallel2 tests/test_parallel2.cpp DEPENDS asyncply PTHREADS)
cmaki_google_test(parallel_sequence tests/test_parallel_sequence.cpp DEPENDS asyncply PTHREADS)
cmaki_google_test(sequence tests/test_sequence.cpp DEPENDS asyncply PTHREADS)
cmaki_google_test(parallel_fes tests/test_parallel_fes.cpp DEPENDS asyncply fes PTHREADS)

