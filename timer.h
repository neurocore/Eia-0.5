#pragma once
#include <chrono>

class Timer
{
    using Clock = std::chrono::high_resolution_clock;
    using MS = std::chrono::milliseconds;
    Clock::time_point clock;

public:
    Timer() {}
    void set() { clock = Clock::now(); }
    unsigned long long getms() const
    {
        return std::chrono::duration_cast<MS>(Clock::now() - clock).count();
    }
};