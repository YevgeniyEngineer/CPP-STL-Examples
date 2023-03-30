cmake_minimum_required(VERSION 3.18)

project(c_plus_plus_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Standard Library (STL)
add_subdirectory(./standard_library_examples)

# Algorithms
add_subdirectory(./algorithm_examples)

# OpenGL
add_subdirectory(./opengl_examples)

# ImGUI
# add_subdirectory(./imgui_examples)
