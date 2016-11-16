cmake_minimum_required(VERSION 2.8)

cmaki2_gtest(bench test_bench.cpp DEPENDS asyncply)
cmaki2_gtest(sequence test_sequence.cpp DEPENDS asyncply)
cmaki2_gtest(parallel test_parallel.cpp DEPENDS asyncply)
cmaki2_gtest(parallel2 test_parallel2.cpp DEPENDS asyncply)
cmaki2_gtest(parallel_sequence test_parallel_sequence.cpp DEPENDS asyncply)
cmaki2_gtest(consumer_producer test_consumer_producer.cpp DEPENDS asyncply)

# 	# disabling gtest
cmaki_find_package(fast-event-system)
cmaki2_gtest(parallel_fes test_parallel_fes.cpp DEPENDS asyncply)

cmaki_find_package(boost-headers)
cmaki_find_package(boost-coroutine2)
cmaki_find_package(boost-filesystem)
cmaki_find_package(boost-regex)
cmaki2_gtest(coro test_coro.cpp DEPENDS asyncply)
