cmake_minimum_required(VERSION 3.18)

project(standard_library_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(TBB REQUIRED)

add_executable(vector_benchmarks_simple vector_benchmarks_simple.cpp)
target_link_libraries(vector_benchmarks_simple TBB::tbb)

add_executable(vector_benchmarks_complex vector_benchmarks_complex.cpp)
target_link_libraries(vector_benchmarks_complex TBB::tbb)

add_executable(threaded_task_queue threaded_task_queue.cpp)

add_executable(detached_threads detached_threads.cpp)
add_executable(thread_safe_queue thread_safe_queue.cpp)

add_executable(producer_consumer producer_consumer.cpp)
add_executable(multi_producer_multi_consumer multi_producer_multi_consumer.cpp)


