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
# set( BUILD_SHARED_LIBS ON CACHE BOOL  "GLFW dynamic lib" )
add_subdirectory( deps/glfw )

# add glm
add_subdirectory( deps/glm )

# add the glad lib from https://glad.dav1d.de/
add_library(glad STATIC deps/glad/src/glad.c)
target_include_directories(glad PUBLIC deps/glad/include)

# add the stb_image lib from https://github.com/nothings/stb/tree/master
add_library(stb_image STATIC deps/stb_image/src/stb_image_impl.c)
target_include_directories(stb_image PUBLIC deps/stb_image/include)

# compile with debug symbols and without optimization so we hit breakpoints
set(CMAKE_BUILD_TYPE Debug)

# add the executable
add_executable(Gyokuro
    src/main.cpp
    src/renderer/Renderer.cpp
    src/renderer/Shader.cpp
    src/renderer/Texture2D.cpp
    src/mesh/Mesh.cpp
    src/mesh/Quad.cpp
    src/mesh/Cube.cpp
    src/camera/Camera.cpp
    src/camera/CameraNode.cpp
    src/camera/FlyCamera.cpp
    src/scene/SceneNode.cpp
    src/scene/SceneController.cpp)

# include any headers
include_directories(src)

# link with our dependencies
target_link_libraries(Gyokuro
    glad
    glfw
    stb_image
    glm)

# copy resources using a custom command to execute during the build process
set(SHADER_SOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
set(SHADER_BUILD_DIR "${CMAKE_BINARY_DIR}/resources")
add_custom_command(
    TARGET Gyokuro POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SHADER_SOURCE_DIR} ${SHADER_BUILD_DIR}
)
