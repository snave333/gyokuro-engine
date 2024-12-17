
#include <renderer/Renderer.h>

#include <glad/glad.h>

Renderer::Renderer(GLFWwindow* window) {
    this->window = window;
}

Renderer::~Renderer() {
    this->window = nullptr;
}

void Renderer::BeginFrame() {

}

void Renderer::Clear() {
    // rendering commands
    glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::EndFrame() {
    
}
