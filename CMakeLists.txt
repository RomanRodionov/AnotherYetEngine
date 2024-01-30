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

set(SOURCES
    src/model.cpp 
    src/texture.cpp 
    src/common.cpp 
    src/camera.cpp 
    src/load_shader.cpp 
    src/window.cpp  
    src/main.cpp
)

set(IMGUI_SOURCES
    include/imgui/imgui.cpp
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imgui_impl_opengl3.cpp
    include/imgui/imgui_impl_glfw.cpp
)

add_executable(TinyEngine ${SOURCES} ${IMGUI_SOURCES})

target_include_directories(TinyEngine PRIVATE libs/assimp-src/include)
target_include_directories(TinyEngine PRIVATE include/imgui)
target_link_libraries(TinyEngine assimp)
target_link_libraries(TinyEngine OpenGL GLEW::glew glfw)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
