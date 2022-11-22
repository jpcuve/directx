#pragma once
#include "stdafx.h"

class Timer {
public:
    void reset();
    void tick();
    double deltaTime() const; // time between 2 ticks
private:
    bool stopped = true;
    long long countsPerSecond = 0;
    long long now = 0;
    long long last = 0;
};
