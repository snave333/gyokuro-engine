
#include <iostream>

#include <renderer/Renderer.h>
#include <renderer/ScreenQuad.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Renderer::Renderer(GLFWwindow* window, const int& width, const int& height) {
    this->window = window;
    size = glm::vec2(width, height);

    CreateFrameBuffer();

    PrintGLInfo();

    // enable backface fulling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glPolygonMode(GL_FRONT, GL_FILL);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment to draw in wireframe
}

Renderer::~Renderer() {
    window = nullptr;

    delete screenQuad;
    screenQuad = nullptr;

    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &textureColorbuffer);
    glDeleteRenderbuffers(1, &depthRenderbuffer);
}

void Renderer::CreateFrameBuffer() {
    screenQuad = new ScreenQuad();
    
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // generate texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // render buffer object for depth / stencil
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::RENDERER::FRAMEBUFFER_NOT_COMPLETE" << std::endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
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
    // bind and clear our frame buffer

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame() {
    // unbind our framebuffer, and render the full screen quad

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    glDisable(GL_DEPTH_TEST);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    screenQuad->Draw(textureColorbuffer);

    glfwSwapBuffers(window);
}
