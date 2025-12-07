
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <gyo/core/Engine.h>
#include <gyo/renderer/Renderer.h>
#include <gyo/scene/SceneController.h>
#include <gyo/resources/Resources.h>
#include <gyo/utilities/GetError.h>
#include <gyo/utilities/Log.h>

namespace gyo {

Engine* Engine::Instance = nullptr;

Engine::Engine(unsigned int ptWidth, unsigned int ptHeight, unsigned int msaaSamples) {
    if(Engine::Instance) {
        throw std::runtime_error("Cannot have 2 instances of Engine");
    }
    Engine::Instance = this;

    glfwSetErrorCallback(Engine::glfwOnError);

    if (!glfwInit()) {
        LOGE("GLFW initialization failed");
        isRunning = true;
        return;
    }

    // configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create the window object; if no dimensions are specified, go full screen
    if(ptWidth == 0 || ptHeight == 0) {
        GLFWmonitor* primaryMoniter = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMoniter);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        window = glfwCreateWindow(mode->width, mode->height, "Gyokuro", primaryMoniter, nullptr);
    }
    else {
        window = glfwCreateWindow(ptWidth, ptHeight, "Gyokuro", nullptr, nullptr);
    }

    // make the window context the main context
    if (window == nullptr) {
        LOGE("Failed to create GLFW window");
        glfwTerminate();
        isRunning = true;
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    // initialize GLAD before we call any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOGE("Failed to initialize GLAD");
        glfwTerminate();
        isRunning = true;
        return;
    }

    // init our frame time query now that glad is initialized
    gpuTimer.Initialize();

    // set our gl window size
    int pxWidth, pxHeight;
    glfwGetFramebufferSize(window, &pxWidth, &pxHeight);
    glViewport(0, 0, pxWidth, pxHeight);
    glCheckError();
    
    // listen for resize event
    glfwSetFramebufferSizeCallback(window, Engine::glfwOnResize);

    // capture the mouse cursor, and listen for position updates
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, Engine::glfwOnMouseMove);

    // finally, initialize our core Gyokuro classes

    Resources::Initialize();

    renderer = new Renderer(pxWidth, pxHeight, msaaSamples);
    sceneController = new SceneController(renderer, pxWidth, pxHeight);

    isRunning = true;
}

Engine::~Engine() {
    Resources::Dispose();

    // clean up
    delete sceneController;
    delete renderer;

    glfwDestroyWindow(window);
    glfwTerminate();

    window = nullptr;
}

void Engine::Frame() {
    if(glfwWindowShouldClose(window)) {
        isRunning = false;
        return;
    }
    
    renderer->stats.Reset();

    // update our delta time
    double currentTime = glfwGetTime();
    double dt = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;
    renderer->stats.frame = dt * 1000.0;

    // input
    processInput(window, dt);

    // CPU update
    sceneController->Update(dt);

    // render our scene
    gpuTimer.StartQuery();
    sceneController->Render();
    gpuTimer.EndQuery();

    renderer->stats.gpuMs = gpuTimer.lastGPUMs;

    // swap the buffers and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Engine::glfwOnError(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void Engine::glfwOnResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glCheckError();

    // TODO call a Resize(width, height) function on sceneController and renderer
}

void Engine::glfwOnMouseMove(GLFWwindow* window, double xpos, double ypos) {
    if(Engine::Instance == nullptr || Engine::Instance->sceneController == nullptr) {
        return;
    }

    Engine::Instance->sceneController->OnMouseMove(xpos, ypos);
}

void Engine::processInput(GLFWwindow *window, float dt) {
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
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        sceneController->OnKeyPressed(GLFW_KEY_E, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        sceneController->OnKeyPressed(GLFW_KEY_Q, dt);
    }
}

} // namespace gyo
