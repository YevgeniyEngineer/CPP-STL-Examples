cmake_minimum_required(VERSION 3.12)

project(vector_of_struct_py)

set(CMAKE_CXX_STANDARD 17)

find_package(pybind11 REQUIRED)

pybind11_add_module(vector_of_struct_py vector_of_struct.cpp bindings.cpp)