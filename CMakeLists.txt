cmake_minimum_required(VERSION 3.18)

project(cpp_stl_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(TBB REQUIRED)

include_directories(./include/)

# "vector_benchmarks_simple" executable
add_executable(vector_benchmarks_simple src/vector_benchmarks_simple.cpp)
target_link_libraries(vector_benchmarks_simple TBB::tbb)

# "vector_benchmarks_complex" executable
add_executable(vector_benchmarks_complex src/vector_benchmarks_complex.cpp)
target_link_libraries(vector_benchmarks_complex TBB::tbb)

add_executable(threaded_task_queue src/threaded_task_queue.cpp)

add_executable(detached_threads src/detached_threads.cpp)
add_executable(thread_safe_queue src/thread_safe_queue.cpp)

add_executable(producer_consumer src/producer_consumer.cpp)