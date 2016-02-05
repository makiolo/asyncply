cmake_minimum_required(VERSION 2.8)

cmaki_test(bench test_bench.cpp DEPENDS asyncply)
cmaki_test(sequence test_bench.cpp DEPENDS asyncply)
cmaki_test(parallel test_bench.cpp DEPENDS asyncply)
cmaki_test(parallel_sequence test_bench.cpp DEPENDS asyncply)
cmaki_test(consumer_producer test_bench.cpp DEPENDS asyncply)

