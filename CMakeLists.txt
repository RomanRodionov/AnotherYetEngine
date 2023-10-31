cmake_minimum_required(VERSION 3.5.0)
project(TinyEngine VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(TinyEngine src/texture.cpp src/common.cpp src/camera.cpp src/load_shader.cpp src/window.cpp  src/main.cpp)

target_link_libraries(TinyEngine OpenGL GLEW::glew glfw)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
