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
        include/gyo/core/Engine.h
        include/gyo/drawable/AABBWireframe.h
        include/gyo/drawable/TangentsRenderer.h
        include/gyo/lighting/DirectionalLight.h
        include/gyo/lighting/LightNode.h
        include/gyo/lighting/PointLight.h
        include/gyo/lighting/SpotLight.h
        include/gyo/mesh/Cube.h
        include/gyo/mesh/Mesh.h
        include/gyo/mesh/Model.h
        include/gyo/mesh/ModelNode.h
        include/gyo/mesh/Pyramid.h
        include/gyo/mesh/Quad.h
        include/gyo/mesh/Skybox.h
        include/gyo/mesh/Sphere.h
        include/gyo/mesh/Torus.h
        include/gyo/resources/Resources.h
        include/gyo/scene/SceneController.h
        include/gyo/shading/GoochMaterial.h
        include/gyo/shading/PhongMaterial.h
        include/gyo/shading/UnlitMaterial.h
        include/gyo/gyo.h
)

set(PRIVATE_HEADERS
        include/gyo/internal/drawable/IDrawable.h
        include/gyo/internal/lighting/Light.h
        include/gyo/internal/mesh/Geometry.h
        include/gyo/internal/renderer/RenderType.h
        include/gyo/internal/scene/SceneNode.h
        include/gyo/internal/shading/Material.h
        include/gyo/internal/shading/Shader.h
        include/gyo/internal/shading/Texture2D.h
        include/gyo/internal/shading/TextureCube.h
        include/gyo/internal/ui/Font.h
        include/gyo/internal/ui/Text.h
        src/camera/Camera.h
        src/camera/CameraNode.h
        src/camera/FlyCamera.h
        src/glad/glad.h
        src/glad/khrplatform.h
        src/math/AABB.h
        src/math/Frustum.h
        src/math/Plane.h
        src/math/Sphere.h
        src/glad/LightsUBO.h
        src/renderer/DrawCall.h
        src/renderer/Renderer.h
        src/renderer/RenderState.h
        src/renderer/ScreenQuad.h
        src/resources/FontLoader.h
        src/resources/ShaderLoader.h
        src/resources/TextureLoader.h
        src/resources/ModelLoader.h
        src/stb_image/stb_image.h
        src/utilities/Clock.h
        src/utilities/FileSystem.h
        src/utilities/FrameStats.h
        src/utilities/Hash.h
)

set(SOURCES
        src/camera/Camera.cpp
        src/camera/CameraNode.cpp
        src/camera/FlyCamera.cpp
        src/core/Engine.cpp
        src/drawable/AABBWireframe.cpp
        src/drawable/TangentsRenderer.cpp
        src/glad/glad.c
        src/lighting/LightNode.cpp
        src/lighting/LightsUBO.cpp
        src/mesh/Mesh.cpp
        src/mesh/Model.cpp
        src/mesh/ModelNode.cpp
        src/mesh/Skybox.cpp
        src/renderer/Renderer.cpp
        src/renderer/RenderState.cpp
        src/renderer/ScreenQuad.cpp
        src/resources/FontLoader.cpp
        src/resources/ModelLoader.cpp
        src/resources/Resources.cpp
        src/resources/ShaderLoader.cpp
        src/resources/TextureLoader.cpp
        src/scene/SceneController.cpp
        src/scene/SceneNode.cpp
        src/shading/GoochMaterial.cpp
        src/shading/PhongMaterial.cpp
        src/shading/Shader.cpp
        src/shading/Texture2D.cpp
        src/shading/TextureCube.cpp
        src/shading/UnlitMaterial.cpp
        src/stb_image/stb_image.c
        src/ui/Font.cpp
        src/ui/Text.cpp
)

# ---- Add our dependencies -----

# configure and add glfw
set( GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL "GLFW lib only" )
set( GLFW_BUILD_EXAMPLES ON CACHE BOOL "Build example executables" )
add_subdirectory( deps/glfw )

# add glm and freetype
add_subdirectory( deps/glm )
add_subdirectory( deps/freetype )

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
include_directories(include)
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
        freetype
)

# ----- Build our samples -----

# build the samples (conditionally)
option(GYO_BUILD_SAMPLES "Build sample executables" ON)
if(GYO_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
