
#include <iostream>

#include <renderer/Renderer.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Renderer::Renderer(GLFWwindow* window) {
    this->window = window;

    PrintGLInfo();

    // enable depth testing and backface fulling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // enable msaa
    glEnable(GL_MULTISAMPLE);
    
    //glPolygonMode(GL_FRONT, GL_LINE); // uncomment to draw in wireframe
    glPolygonMode(GL_FRONT, GL_FILL);
}

Renderer::~Renderer() {
    window = nullptr;
}

void Renderer::PrintGLInfo() {
    std::cout << "Renderer Info & Capabilities: " << std::endl;

    // Query and print renderer and version information
    const GLubyte* renderer = glGetString(GL_RENDERER);  // GPU name
    const GLubyte* version = glGetString(GL_VERSION);    // OpenGL version
    const GLubyte* vendor = glGetString(GL_VENDOR);      // Vendor name
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION); // GLSL version

    std::cout << "- Renderer: " << renderer << "\n";
    std::cout << "- OpenGL Version: " << version << "\n";
    std::cout << "- Vendor: " << vendor << "\n";
    std::cout << "- GLSL Version: " << glslVersion << "\n";

    // Query and print OpenGL limits and capabilities
    GLint value;
    
    // Shader and Program Limits
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
    std::cout << "- Max Vertex Attributes: " << value << "\n";

    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &value);
    std::cout << "- Max Vertex Uniform Components: " << value << "\n";

    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &value);
    std::cout << "- Max Fragment Uniform Components: " << value << "\n";

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
    std::cout << "- Max Texture Image Units: " << value << "\n";

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &value);
    std::cout << "- Max Combined Texture Image Units: " << value << "\n";

    glGetIntegerv(GL_MAX_VARYING_COMPONENTS, &value);
    std::cout << "- Max Varying Components: " << value << "\n";

    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &value);
    std::cout << "- Max Uniform Block Size: " << value << " bytes\n";

    // Texture Capabilities
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
    std::cout << "- Max 2D Texture Size: " << value << "x" << value << "\n";

    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &value);
    std::cout << "- Max 3D Texture Size: " << value << "x" << value << "x" << value << "\n";

    glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &value);
    std::cout << "- Max Cube Map Texture Size: " << value << "x" << value << "\n";

    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &value);
    std::cout << "- Max Array Texture Layers: " << value << "\n";

    // Framebuffer and Renderbuffer
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &value);
    std::cout << "- Max Renderbuffer Size: " << value << "x" << value << "\n";

    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &value);
    std::cout << "- Max Color Attachments: " << value << "\n";

    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &value);
    std::cout << "- Max Draw Buffers: " << value << "\n";

    // MSAA Capabilities
    glGetIntegerv(GL_MAX_SAMPLES, &value);
    std::cout << "- Max MSAA Samples: " << value << "\n";
#if false
    // Compute Shader Limits (if supported)
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &value);
    std::cout << "- Max Compute Work Group Invocations: " << value << "\n";

    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, &value);
    std::cout << "- Max Compute Work Group Count (x, y, z): (" << value << ", " << value << ", " << value << ")\n";
#endif
    // Viewport and Clipping
    GLint viewportDims[2];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, viewportDims);
    std::cout << "- Max Viewport Dimensions: " << viewportDims[0] << "x" << viewportDims[1] << "\n";

    glGetIntegerv(GL_MAX_CLIP_DISTANCES, &value);
    std::cout << "- Max Clip Distances: " << value << "\n";

    // Extensions
    const GLubyte* extensions = glGetString(GL_EXTENSIONS);
    if(extensions != 0) {
        std::cout << "- Extensions:\n" << extensions << "\n";
    }
}

void Renderer::BeginFrame() {

}

void Renderer::Clear() {
    glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame() {    
    glfwSwapBuffers(window);
}
