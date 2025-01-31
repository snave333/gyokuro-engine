#ifndef ENGINE_H
#define ENGINE_H

struct GLFWwindow;

namespace gyo {

class SceneController;
class Renderer;

class Engine {
public:
    static Engine* Instance;
    
public:
    Engine(unsigned int ptWidth, unsigned int ptHeight);
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
    float dt;
    float fps;
    float currentTime;
    float lastUpdateTime;

    // our glfw callbacks
    static void glfwOnError(int error, const char* description);
    static void glfwOnResize(GLFWwindow* window, int width, int height);
    static void glfwOnMouseMove(GLFWwindow* window, double xpos, double ypos);

    void processInput(GLFWwindow *window, float dt);
    void checkShouldExit(GLFWwindow *window);
};

} // namespace gyo

#endif // ENGINE_H
