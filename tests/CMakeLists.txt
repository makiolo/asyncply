cmake_minimum_required(VERSION 2.8)

cmaki_test(bench test_bench.cpp DEPENDS asyncply)
cmaki_test(sequence test_sequence.cpp DEPENDS asyncply)
cmaki_test(parallel test_parallel.cpp DEPENDS asyncply)
cmaki_test(parallel2 test_parallel2.cpp DEPENDS asyncply)
cmaki_test(parallel_sequence test_parallel_sequence.cpp DEPENDS asyncply)
cmaki_test(consumer_producer test_consumer_producer.cpp DEPENDS asyncply)
cmaki_test(coroutine test_coroutine.cpp DEPENDS asyncply)

