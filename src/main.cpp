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

void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

Renderer* renderer;
SceneController* sceneController;

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

    // enable msaa
    glfwWindowHint(GLFW_SAMPLES, 4);

    // create the window object, and make the window context the main context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Gyokuro", NULL, NULL);
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

    // our rendering managers
    renderer = new Renderer(window);
    sceneController = new SceneController(renderer);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        sceneController->OnKeyboardInput(key, action, mods);
    });

    // timing
    float lastUpdateTime = glfwGetTime();
    float dt, fps;

    // our render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // update our delta time
        float currentTime = glfwGetTime();
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

    // clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    
    delete sceneController;
    delete renderer;
    
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

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
