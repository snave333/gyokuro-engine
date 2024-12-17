cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Gyokuro VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# require the OpenGL framework
find_package(OpenGL REQUIRED)

# configure and add glfw
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set( BUILD_SHARED_LIBS ON CACHE BOOL  "GLFW dynamic lib" )
add_subdirectory( deps/glfw )

# add the downloaded glad lib from https://glad.dav1d.de/
add_library(glad STATIC deps/glad/src/glad.c)
target_include_directories(glad PUBLIC deps/glad/include)

# compile with debug symbols and without optimization so we hit breakpoints
set(CMAKE_BUILD_TYPE Debug)

# add the executable
add_executable(Gyokuro
    src/main.cpp
    src/renderer/Renderer.cpp
    src/scene/SceneController.cpp)

# include any headers
include_directories(src)

# link with our dependencies
target_link_libraries(Gyokuro glad glfw)
