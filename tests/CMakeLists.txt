cmake_minimum_required(VERSION 2.8)

find_package(verypoco REQUIRED)
CREATE_TEST(test_bench "")
CREATE_TEST(test_sequence "")
CREATE_TEST(test_parallel "")
CREATE_TEST(test_parallel_sequence "")
CREATE_TEST(test_consumer_producer "")
target_link_libraries(test_bench verypoco multithread)
target_link_libraries(test_sequence verypoco multithread)
target_link_libraries(test_parallel verypoco multithread)
target_link_libraries(test_parallel_sequence verypoco multithread)
target_link_libraries(test_consumer_producer verypoco multithread)

