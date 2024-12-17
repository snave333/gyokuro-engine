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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create the window object, and make the window context the main context
    GLFWwindow* window = glfwCreateWindow(512, 512, "Gyokuro", NULL, NULL);
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
    Renderer* renderer = new Renderer(window);
    SceneController* sceneController = new SceneController(renderer);

    // our render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        sceneController->Update(0.0);
        sceneController->Render();
        
        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    
    delete sceneController;
    delete renderer;
    
    return 0;
}
