#pragma once

#include <chrono>

class Timer {
public:
    inline double getElapsedTime() { return elapsed_time.count(); }
    inline void startTimer() { start_time = std::chrono::steady_clock::now(); }
    inline void stopTimer() { elapsed_time = std::chrono::steady_clock::now() - start_time; }
private:    
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_time = std::chrono::duration<double>::zero();
};