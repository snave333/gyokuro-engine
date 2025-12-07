#ifndef FRAME_TIMER_H
#define FRAME_TIMER_H

#include <glad/glad.h>

#include <gyo/utilities/GetError.h>

namespace gyo {

class FrameTimer {
public:
    FrameTimer() {}

    ~FrameTimer() {
        if(!initialized) {
            return;
        }

        glDeleteQueries(2, gpuQueries);
        glCheckError();
    }
    
    void Initialize() {
        if(initialized) {
            return;
        }

        glGenQueries(2, gpuQueries);
        glCheckError();

        initialized = true;
    }

    void StartQuery() {
        int previous = (frameIndex + 1) % 2;
        int current = frameIndex % 2;

        // first read our previous frame's results

        GLuint available = 0;
        glGetQueryObjectuiv(gpuQueries[previous], GL_QUERY_RESULT_AVAILABLE, &available);
        glCheckError();

        if(available) {
            GLuint64 ns = 0;
            glGetQueryObjectui64v(gpuQueries[previous], GL_QUERY_RESULT, &ns);
            glCheckError();
            lastGPUMs = float(ns) / 1e6; // ns to ms
        }

        // now begin timing this frame

        glBeginQuery(GL_TIME_ELAPSED, gpuQueries[current]);
        glCheckError();
    }

    void EndQuery() {
        glEndQuery(GL_TIME_ELAPSED);
        glCheckError();

        frameIndex++;
    }

    bool initialized = false;
    float lastGPUMs = 0;

private:
    GLuint gpuQueries[2] = {0, 0};

    unsigned int frameIndex = 0;
};

} // namespace gyo

#endif // FRAME_TIMER_H
