cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Gyokuro VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# require the OpenGL framework
find_package(OpenGL REQUIRED)

# configure and add glfw
set( GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL "GLFW lib only" )
add_subdirectory( deps/glfw )

# add glm and freetype
add_subdirectory( deps/glm )
add_subdirectory( deps/freetype )

# configure and add assimp asset importer
set( ASSIMP_NO_EXPORT ON CACHE BOOL "Importers only" )
set( ASSIMP_BUILD_TESTS OFF CACHE BOOL "Don't build tests" )
set( ASSIMP_INSTALL OFF CACHE BOOL "We're using as a submodule" )
add_subdirectory( deps/assimp )

# add the glad lib from https://glad.dav1d.de/
add_library(glad STATIC deps/glad/src/glad.c)
target_include_directories(glad PUBLIC deps/glad/include)

# add the stb_image lib from https://github.com/nothings/stb/
add_library(stb_image STATIC deps/stb_image/src/stb_image.c)
target_include_directories(stb_image PUBLIC deps/stb_image/include)

# libjpeg for jpeg decompression
set( ENABLE_SHARED OFF CACHE BOOL "Disable unused libjpeg shared libs" )
set( WITH_TURBOJPEG OFF CACHE BOOL "Don't use libturbojpeg (uses SIMD)" )
include(ExternalProject)
ExternalProject_Add(
    jpeglib
    PREFIX ${CMAKE_BINARY_DIR}/external
    GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
    GIT_TAG 3.1.0
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> 
        -DENABLE_SHARED=OFF
        -DWITH_SIMD=OFF
        -DWITH_TURBOJPEG=OFF
        -DCMAKE_BUILD_TYPE=Debug
)
ExternalProject_Get_Property(jpeglib install_dir)
set(JPEGLIB_INCLUDE_DIR ${install_dir}/include)
set(JPEGLIB_LIBRARY ${install_dir}/lib/libjpeg.a)

# compile with debug symbols and without optimization so we hit breakpoints
set(CMAKE_BUILD_TYPE Debug)

# add the executable
add_executable(Gyokuro
    src/main.cpp
    src/renderer/Renderer.cpp
    src/renderer/ScreenQuad.cpp
    src/renderer/RenderState.cpp
    src/shading/UnlitMaterial.cpp
    src/shading/PhongMaterial.cpp
    src/shading/GoochMaterial.cpp
    src/shading/Shader.cpp
    src/shading/Texture2D.cpp
    src/shading/TextureCube.cpp
    src/lighting/LightNode.cpp
    src/lighting/LightsUBO.cpp
    src/mesh/Mesh.cpp
    src/mesh/Model.cpp
    src/mesh/ModelNode.cpp
    src/mesh/Skybox.cpp
    src/drawable/AABBWireframe.cpp
    src/drawable/TangentsRenderer.cpp
    src/camera/Camera.cpp
    src/camera/CameraNode.cpp
    src/camera/FlyCamera.cpp
    src/scene/SceneNode.cpp
    src/scene/SceneController.cpp
    src/resources/Resources.cpp
    src/resources/ModelLoader.cpp
    src/resources/ShaderLoader.cpp
    src/resources/TextureLoader.cpp
    src/resources/FontLoader.cpp
    src/ui/Font.cpp
    src/ui/Text.cpp)

# include headers
target_include_directories(Gyokuro PRIVATE src)
target_include_directories(Gyokuro PRIVATE ${JPEGLIB_INCLUDE_DIR})

# link with our dependencies
target_link_libraries(Gyokuro
    glad
    glfw
    stb_image
    ${JPEGLIB_LIBRARY}
    assimp
    glm
    freetype)

# set shader source and build directories
set(RESOURCES_SRC_DIR "${CMAKE_SOURCE_DIR}/resources")
set(RESOURCES_DST_DIR "${CMAKE_BINARY_DIR}/resources")

# add a custom target to copy resources
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${RESOURCES_SRC_DIR} ${RESOURCES_DST_DIR}
    COMMENT "Copying resources to build directory"
)

# ensure Gyokuro depends on the copy_resources target
add_dependencies(Gyokuro copy_resources)
