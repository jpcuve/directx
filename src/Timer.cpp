#include "Timer.h"

void Timer::tick() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    last = now;
    now = li.QuadPart;
}

double Timer::deltaTime() const {
    auto ticks = now - last;
    if (ticks < 0){
        ticks = 0;
    }
    return static_cast<double>(ticks) / static_cast<double>(countsPerSecond);
}

void Timer::reset() {
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    countsPerSecond = li.QuadPart;
    QueryPerformanceCounter(&li);
    now = last = li.QuadPart;
}
