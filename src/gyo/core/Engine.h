#ifndef ENGINE_H
#define ENGINE_H

#include <gyo/utilities/FrameTimer.h>

struct GLFWwindow;

namespace gyo {

class SceneController;
class Renderer;

class Engine {
public:
    static Engine* Instance;
    
public:
    Engine(unsigned int ptWidth = 0, unsigned int ptHeight = 0, unsigned int msaaSamples = 4U);
    ~Engine();

    const bool& IsRunning() const { return isRunning; }
    void Frame();
    void ShutDown() { isRunning = false; }

    SceneController& sc() { return *sceneController; }

private:
    GLFWwindow* window;
    Renderer* renderer;
    SceneController* sceneController;

    bool isRunning = false;

    // timing
    double lastUpdateTime;
    FrameTimer gpuTimer;

    // our glfw callbacks
    static void glfwOnError(int error, const char* description);
    static void glfwOnResize(GLFWwindow* window, int width, int height);
    static void glfwOnMouseMove(GLFWwindow* window, double xpos, double ypos);

    void processInput(GLFWwindow *window, float dt);
};

} // namespace gyo

#endif // ENGINE_H
