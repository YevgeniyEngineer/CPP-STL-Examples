cmake_minimum_required(VERSION 3.18)

project(algorithm_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)

add_executable(delaunay_triangulation delaunay_triangulation.cpp)

target_include_directories(delaunay_triangulation
    PRIVATE
        python3.10
        ${Python_INCLUDE_DIRS}
        ${Python_NumPy_INCLUDE_DIRS}
        plotting
)

target_link_libraries(delaunay_triangulation
    Python3::Python
    Python3::Module
    Python3::NumPy
)

add_executable(quad_tree quad_tree.cpp)
