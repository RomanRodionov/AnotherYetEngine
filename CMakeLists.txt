cmake_minimum_required(VERSION 3.5.0)
project(TinyEngine VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/libs CACHE PATH "Missing description." FORCE)
FetchContent_Declare(assimp
GIT_REPOSITORY https://github.com/assimp/assimp.git
GIT_TAG master)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)

include(CTest)
enable_testing()

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(TinyEngine src/model.cpp src/texture.cpp src/common.cpp src/camera.cpp src/load_shader.cpp src/window.cpp  src/main.cpp)

target_include_directories(TinyEngine PRIVATE libs/assimp-src/include)
target_link_libraries(TinyEngine assimp)
target_link_libraries(TinyEngine OpenGL GLEW::glew glfw)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
