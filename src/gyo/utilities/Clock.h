#ifndef CLOCK_H
#define CLOCK_H

#include <gyo/utilities/Log.h>

#include <iostream>
#include <cstdio>
#include <string>
#include <chrono>

namespace gyo {

#define CLOCK(name) Clock clock_##name(#name)
#define CLOCKT(name, t) Clock clock_##name(#name, t)

class Clock {
public:
    Clock(const char* n) : name(n) {
        start = std::chrono::high_resolution_clock::now();
    }

    Clock(const char* n, float* t) : name(n), timeMs(t) {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Clock() {
        auto end = std::chrono::high_resolution_clock::now();

        float ms = std::chrono::duration<float, std::milli>(end - start).count();

        if(timeMs != nullptr) {
            *timeMs = ms;
        }
        else {
            LOGI("%s: %.2f ms", name.c_str(), ms);
        }
    }

private:
    std::string name;
    std::chrono::high_resolution_clock::time_point start;
    float* timeMs = nullptr;
};

} // namespace gyo

#endif // CLOCK_H
