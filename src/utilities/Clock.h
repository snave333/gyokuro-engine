#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <cstdio>
#include <string>
#include <chrono>

#define CLOCK(name) Clock clock##name(#name)

class Clock {
public:
    Clock(const char* n) : name(n) {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Clock() {
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsedNs = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        float ms = (float)elapsedNs / 1e+6;

        // std::cout << name << ": " << ms << " ms" << std::endl;
        printf("%s: %.2f ms\n", name.c_str(), ms);
    }

private:
    std::string name;
    std::chrono::high_resolution_clock::time_point start;
};

#endif // CLOCK_H
