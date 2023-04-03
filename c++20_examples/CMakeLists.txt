cmake_minimum_required(VERSION 3.18)

project(cpp_20_examples)

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

add_executable(optional optional.cpp)
add_executable(any any.cpp)