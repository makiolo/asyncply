cmake_minimum_required(VERSION 2.8)

cmaki_find_package(teelogging)
cmaki2_gtest(bench test_bench.cpp DEPENDS asyncply PTHREADS)
cmaki2_gtest(parallel test_parallel.cpp DEPENDS asyncply PTHREADS)
cmaki2_gtest(consumer_producer test_consumer_producer.cpp DEPENDS asyncply PTHREADS)
cmaki2_gtest(parallel2 test_parallel2.cpp DEPENDS asyncply PTHREADS)
cmaki2_gtest(parallel_sequence test_parallel_sequence.cpp DEPENDS asyncply PTHREADS)
cmaki2_gtest(sequence test_sequence.cpp DEPENDS asyncply PTHREADS)

cmaki_find_package(fast-event-system)
cmaki2_gtest(parallel_fes test_parallel_fes.cpp DEPENDS asyncply PTHREADS)

