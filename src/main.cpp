//
//  main.cpp
//  Gyokuro
//
//  Created by Spencer Evans on 12/16/24.
//

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <renderer/Renderer.h>
#include <scene/SceneController.h>
#include <resources/Resources.h>

void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, float dt);
void onMouseInput(GLFWwindow* window, double xpos, double ypos);

Renderer* renderer;
SceneController* sceneController;
float dt, fps;

int main(int argc, const char * argv[]) {
    std::cout << "Starting..." << std::endl;
    
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit()) {
        std::cout << "GLFW initialization failed" << std::endl;
        return -1;
    }

    // configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create the window object, and make the window context the main context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Gyokuro", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // initialize GLAD before we call any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // set our gl window size
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    // listen for resize event
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // capture the mouse cursor, and listen for position updates
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, onMouseInput);

    Resources::Initialize();

    // our rendering managers
    renderer = new Renderer(window, width, height);
    sceneController = new SceneController(renderer, width, height);

    // timing
    float currentTime;
    float lastUpdateTime = glfwGetTime();

    // our render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window, dt);

        // update our delta time
        currentTime = glfwGetTime();
        dt = currentTime - lastUpdateTime;
        lastUpdateTime = currentTime;
        if(dt > 0.0f) {
            fps = 1.0f / dt;
        }
        // std::cout << (int)(dt * 1000) << " ms, " << (int)fps << " fps" << std::endl;

        // update and render our scene
        sceneController->Update(dt);
        sceneController->Render();
        
        // check and call events and swap the buffers
        glfwPollEvents();
    }

    Resources::Dispose();

    // clean up
    delete sceneController;
    delete renderer;
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, float dt)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // TODO a better solution for passing input to the scene controller

    if(sceneController == nullptr) {
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        sceneController->OnKeyPressed(GLFW_KEY_W, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        sceneController->OnKeyPressed(GLFW_KEY_S, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        sceneController->OnKeyPressed(GLFW_KEY_A, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        sceneController->OnKeyPressed(GLFW_KEY_D, dt);
    }
}

void onMouseInput(GLFWwindow* window, double xpos, double ypos) {
    if(sceneController == nullptr) {
        return;
    }

    sceneController->OnMouseMove(xpos, ypos);
}
