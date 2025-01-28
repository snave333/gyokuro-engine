# Gyokuro Rendering Library

A C++ OpenGL rendering library – pronounced "gee-o-kuh-roo". Uses the forward shading technique. Targets the OpenGL 3.3 Core profile.

## Roadmap

* [ ] Camera
  * [x] Perspective/orthagraphic projections
  * [x] Free-look, fly camera
  * [ ] Orbit camera
* [x] 3D Primitives
  * [x] Quad
  * [x] Cube
  * [x] Sphere
  * [x] Torus
  * [x] Pyramid
* [x] Shaders
  * [x] GLSL file loading
  * [x] #include "<filename>.glsl" support
  * [x] Camera matrices UBO
  * [x] Scene lighting UBO
* [x] 2D Texture Loading
* [ ] Lights
  * [x] Directional
  * [x] Point
  * [x] Spot
  * [ ] Area
* [x] Material Definitions
  * [x] Unlit (supports color map)
  * [x] Phong (supports diffuse, specular, and normal map)
* [ ] Utility Drawables
  * [x] Mesh normals/tangents/bitangents viewer (geom shader)
  * [x] AABB wireframe renderer
  * [ ] Frustum wireframe renderer
* [ ] Skybox
  * [x] Cubemap
  * [ ] Skydome
* [ ] View Frustum Culling
  * [x] AABB based model culling
    * [x] P/N vertex LUT optimization
    * [x] Plane coherence optimization
  * [ ] Scene bounding volume hierarchy
* [x] Blending
  * [x] Sorted transparency
  * [x] Additive
* [x] Resource Caching
  * [x] Shaders
  * [x] Textures
  * [x] Cubemaps
  * [x] Fonts
* [ ] Screen text rendering
  * [x] .ttf file parsing
  * [x] Glyph rendering
  * [ ] SDF-based text rendering
* [x] HDR
  * [x] Render buffer
  * [x] Reinhard tone mapping
  * [x] Gamma correction
* [x] Model loading
  * [x] Separate model/texture file loading
  * [x] Embedded texture support (glb)
* [ ] PBR
* [ ] Image effects
* [ ] Shadows

## Dependencies

* [glad](https://github.com/Dav1dde/glad)
* [glfw](https://github.com/glfw/glfw/)
* [glm](https://github.com/g-truc/glm/)
* [stb_image](https://github.com/nothings/stb/)
* [freetype](https://github.com/freetype/freetype/)
* [assimp](https://github.com/assimp/assimp/)

## Acknowledgments

* [Computer Graphics Programming In OpenGL With C++](https://www.merclearning.com/titles/Computer_Graphics_Programming_in_OpenGL_with_C++.html)
* [Real-Time Rendering](https://www.realtimerendering.com/)
* [Game Engine Architecture](https://www.gameenginebook.com/)
* [Learn OpenGL](https://learnopengl.com/)
* [3D Game Engine Programming](https://www.3dgep.com/category/graphics-programming/)
* [Optimized View Frustum Culling Algorithms for Bounding Boxes](https://www.cse.chalmers.se/~uffe/vfc_bbox.pdf)
* [Fast Extraction of Viewing Frustum Planes from the WorldView-Projection Matrix](https://www8.cs.umu.se/kurser/5DV051/HT12/lab/plane_extraction.pdf)
* [Other Graphics Programming Papers](https://www.gamedevs.org/#graphics-programming)
