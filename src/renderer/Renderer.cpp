
#include <iostream>

#include <renderer/Renderer.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Renderer::Renderer(GLFWwindow* window) {
    this->window = window;

    // enable depth testing and backface fulling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    //glPolygonMode(GL_FRONT, GL_LINE); // uncomment to draw in wireframe
    glPolygonMode(GL_FRONT, GL_FILL);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
}

Renderer::~Renderer() {
    window = nullptr;
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
