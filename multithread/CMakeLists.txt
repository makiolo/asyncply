cmake_minimum_required(VERSION 2.8)

cmaki_library(multithread	circular_queue.cpp mutex.cpp
							synchronizer.cpp job.cpp pool_thread.cpp
							thread.cpp job_functor.cpp
							sem.cpp worker.cpp)

