cmake_minimum_required(VERSION 3.18)

project(opengl_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_PREFIX_PATH "/usr/local")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(X11 REQUIRED)

add_executable(point_cloud_rendering_static point_cloud_rendering_static.cpp)
target_link_libraries(point_cloud_rendering_static PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${X11_LIBRARIES})

add_executable(point_cloud_rendering_animated point_cloud_rendering_animated.cpp)
target_link_libraries(point_cloud_rendering_animated PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${X11_LIBRARIES})