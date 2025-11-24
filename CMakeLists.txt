cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(gyokuro VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# require the OpenGL framework
find_package(OpenGL REQUIRED)

# compile with debug symbols and without optimization so we hit breakpoints
set(CMAKE_BUILD_TYPE Debug)

# ----- List our header and source files -----

set(PUBLIC_HEADERS
    src/gyo/core/Engine.h
    src/gyo/drawable/AABBWireframe.h
    src/gyo/drawable/TangentsRenderer.h
    src/gyo/geometry/Cube.h
    src/gyo/geometry/Pyramid.h
    src/gyo/geometry/Quad.h
    src/gyo/geometry/Sphere.h
    src/gyo/geometry/Torus.h
    src/gyo/lighting/DirectionalLight.h
    src/gyo/lighting/LightNode.h
    src/gyo/lighting/PointLight.h
    src/gyo/lighting/SpotLight.h
    src/gyo/mesh/Mesh.h
    src/gyo/mesh/Model.h
    src/gyo/mesh/ModelNode.h
    src/gyo/mesh/Skybox.h
    src/gyo/resources/Resources.h
    src/gyo/scene/SceneController.h
    src/gyo/shading/GoochMaterial.h
    src/gyo/shading/PBRMaterial.h
    src/gyo/shading/PhongMaterial.h
    src/gyo/shading/ShaderMaterial.h
    src/gyo/shading/ShaderSemantics.h
    src/gyo/shading/UnlitMaterial.h
    src/gyo/gyo.h
)

set(PRIVATE_HEADERS
    src/glad/glad.h
    src/glad/khrplatform.h
    src/gyo/camera/Camera.h
    src/gyo/camera/CameraNode.h
    src/gyo/camera/FlyCamera.h
    src/gyo/drawable/IDrawable.h
    src/gyo/geometry/Geometry.h
    src/gyo/lighting/Light.h
    src/gyo/lighting/LightsUBO.h
    src/gyo/math/AABB.h
    src/gyo/math/Frustum.h
    src/gyo/math/Plane.h
    src/gyo/math/Sphere.h
    src/gyo/mesh/Vertex.h
    src/gyo/renderer/DrawCall.h
    src/gyo/renderer/Renderer.h
    src/gyo/renderer/RenderState.h
    src/gyo/renderer/RenderType.h
    src/gyo/renderer/ScreenQuad.h
    src/gyo/resources/DataLoader.h
    src/gyo/resources/FontLoader.h
    src/gyo/resources/ShaderLoader.h
    src/gyo/resources/TextureLoader.h
    src/gyo/resources/ModelLoader.h
    src/gyo/scene/SceneNode.h
    src/gyo/shading/Material.h
    src/gyo/shading/Shader.h
    src/gyo/shading/Texture2D.h
    src/gyo/shading/TextureCube.h
    src/gyo/ui/Font.h
    src/gyo/ui/Text.h
    src/gyo/utilities/Clock.h
    src/gyo/utilities/FileSystem.h
    src/gyo/utilities/FrameStats.h
    src/gyo/utilities/GetError.h
    src/gyo/utilities/Hash.h
    src/stb_image/stb_image.h
)

set(SOURCES
    src/glad/glad.c
    src/gyo/camera/Camera.cpp
    src/gyo/camera/CameraNode.cpp
    src/gyo/camera/FlyCamera.cpp
    src/gyo/core/Engine.cpp
    src/gyo/drawable/AABBWireframe.cpp
    src/gyo/drawable/TangentsRenderer.cpp
    src/gyo/lighting/LightNode.cpp
    src/gyo/lighting/LightsUBO.cpp
    src/gyo/mesh/Mesh.cpp
    src/gyo/mesh/Model.cpp
    src/gyo/mesh/ModelNode.cpp
    src/gyo/mesh/Skybox.cpp
    src/gyo/renderer/Renderer.cpp
    src/gyo/renderer/RenderState.cpp
    src/gyo/renderer/ScreenQuad.cpp
    src/gyo/resources/DataLoader.cpp
    src/gyo/resources/FontLoader.cpp
    src/gyo/resources/ModelLoader.cpp
    src/gyo/resources/Resources.cpp
    src/gyo/resources/ShaderLoader.cpp
    src/gyo/resources/TextureLoader.cpp
    src/gyo/scene/SceneController.cpp
    src/gyo/scene/SceneNode.cpp
    src/gyo/shading/GoochMaterial.cpp
    src/gyo/shading/Material.cpp
    src/gyo/shading/PBRMaterial.cpp
    src/gyo/shading/PhongMaterial.cpp
    src/gyo/shading/Shader.cpp
    src/gyo/shading/Texture2D.cpp
    src/gyo/shading/TextureCube.cpp
    src/gyo/shading/ShaderMaterial.cpp
    src/gyo/shading/UnlitMaterial.cpp
    src/gyo/ui/Font.cpp
    src/gyo/ui/Text.cpp
    src/gyo/utilities/GetError.cpp
    src/stb_image/stb_image.c
)

# ---- Add our dependencies -----

# configure and add glfw
set( GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL "GLFW lib only" )
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build example executables" )
add_subdirectory( deps/glfw )

# add glm
add_subdirectory( deps/glm )

# configure and add assimp asset importer
set( ASSIMP_NO_EXPORT ON CACHE BOOL "Importers only" )
set( ASSIMP_BUILD_TESTS OFF CACHE BOOL "Don't build tests" )
set( ASSIMP_INSTALL OFF CACHE BOOL "We're using as a submodule" )
add_subdirectory( deps/assimp )

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

# ----- Add and link our library -----

# add the library
add_library(gyokuro STATIC ${SOURCES})

# include headers
include_directories(src)
target_include_directories(gyokuro PUBLIC ${PUBLIC_HEADERS} PRIVATE ${PRIVATE_HEADERS})
target_include_directories(gyokuro PRIVATE ${JPEGLIB_INCLUDE_DIR})

# link with our dependencies
target_link_libraries(gyokuro
    PUBLIC
        glm
    PRIVATE
        glfw
        ${JPEGLIB_LIBRARY}
        assimp
)

# ----- Build our samples -----

# build the samples (conditionally)
option(GYO_BUILD_SAMPLES "Build sample executables" ON)
if(GYO_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
