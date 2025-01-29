#ifndef CLOCK_H
#define CLOCK_H

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

        auto elapsedNs = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        float ms = (float)elapsedNs / 1e+6;

        if(timeMs != nullptr) {
            *timeMs = ms;
        }
        else {
            // std::cout << name << ": " << ms << " ms" << std::endl;
            printf("%s: %.2f ms\n", name.c_str(), ms);
        }
    }

private:
    std::string name;
    std::chrono::high_resolution_clock::time_point start;
    float* timeMs = nullptr;
};

} // namespace gyo

#endif // CLOCK_H
