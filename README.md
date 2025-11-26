# Gyokuro Rendering Library

A C++ OpenGL rendering library – pronounced "gee-o-kuh-ro". Uses the forward shading technique and targets the OpenGL 3.3 Core profile.

This project is my journey following the tutorials and guides from [Learn OpenGL](https://learnopengl.com/), and creating a high-level wrapper around the rendering software.

## Key Features

 - Model Loading
 - PBR Materials
 - HDR Pipeline
 - Fast View Frustum Culling

For a full feature list and roadmap of planned features, see [the list below](#roadmap).

## Example Usage

```cpp
// start the rendering engine

gyo::Engine* engine = new gyo::Engine(1920, 1080);
if(!engine->IsRunning()) {
    std::cerr << "Gyokuro failed to start!" << std::endl;
    return 1;
}

// create our scene

SceneController& sc = engine->sc();

sc.SetEnvironment("brown_photostudio_2k.hdr");

glm::vec3 spotLightColor = { 1.0f, 0.2f, 0.2f };
LightNode* spotLight = new LightNode(new SpotLight(spotLightColor * 6.0f, 40.0f));
spotLight->Translate(2, 1, -5);
spotLight->Rotate(45, -15, 0);
sc.AddNode(spotLight);

ModelNode* helmet = new ModelNode(Resources::GetModel("DamagedHelmet.glb", true));
sc.AddNode(helmet);
glm::vec3 rotationAxis = { 0.0f, 1.0f, 0.0f };

sc.AddUpdateFunction([helmet, rotationAxis](float dt) {
    helmet->Rotate(dt * 15, rotationAxis);
});

// render the scene to the window

while(engine->IsRunning()) {
    engine->Frame();
}
```

For more examples, see the runnable projects in the [samples](samples) folder.

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
  * [x] #define injection
  * [x] Camera matrices UBO
  * [x] Scene lighting UBO
* [x] 2D Texture Loading
* [x] Simple Lights
  * [x] Directional
  * [x] Point
  * [x] Spot
* [x] Material Definitions
  * [x] Unlit (supports color map)
  * [x] Phong (supports diffuse, specular, and normal map)
* [ ] Utility Drawables
  * [x] Mesh normals/tangents/bitangents viewer (geom shader)
  * [x] AABB wireframe renderer
  * [ ] Frustum wireframe renderer
* [x] Skybox
  * [x] Cubemap
  * [x] HDR
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
* [ ] UI
  * [x] MSDF-based text rendering
  * [ ] UI images/panels
* [x] HDR
  * [x] Render buffer
  * [x] Reinhard tone mapping
  * [x] Gamma correction
* [x] Model loading
  * [x] Separate model/texture file loading
  * [x] Embedded texture support (glb)
* [ ] PBR
  * [ ] Material (supports albedo, normal, metallic/roughness, and ao map)
  * [x] Analytic lights
  * [x] IBL
* [ ] MSAA
* [ ] Image effects
* [ ] Shadows

## Dependencies

* [glad](https://github.com/Dav1dde/glad)
* [glfw](https://github.com/glfw/glfw/)
* [glm](https://github.com/g-truc/glm/)
* [stb_image](https://github.com/nothings/stb/)
* [assimp](https://github.com/assimp/assimp/)

## Acknowledgments

"Dice.fbx" 3D model and textures by [Josh Cook](https://www.linkedin.com/in/josh-cook-380a3a147/).

* [Computer Graphics Programming In OpenGL With C++](https://www.merclearning.com/titles/Computer_Graphics_Programming_in_OpenGL_with_C++.html)
* [Real-Time Rendering](https://www.realtimerendering.com/)
* [Game Engine Architecture](https://www.gameenginebook.com/)
* [Learn OpenGL](https://learnopengl.com/)
* [3D Game Engine Programming](https://www.3dgep.com/category/graphics-programming/)
* [Optimized View Frustum Culling Algorithms for Bounding Boxes](https://www.cse.chalmers.se/~uffe/vfc_bbox.pdf)
* [Fast Extraction of Viewing Frustum Planes from the WorldView-Projection Matrix](https://www8.cs.umu.se/kurser/5DV051/HT12/lab/plane_extraction.pdf)
* [Other Graphics Programming Papers](https://www.gamedevs.org/#graphics-programming)
