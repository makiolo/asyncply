cmake_minimum_required(VERSION 2.8)

find_package(verypoco REQUIRED)
CREATE_TEST("test_bench" "verypoco")
CREATE_TEST("test_sequence" "verypoco")
CREATE_TEST("test_parallel" "verypoco")
CREATE_TEST("test_parallel_sequence" "verypoco")
CREATE_TEST("test_consumer_producer" "verypoco")

