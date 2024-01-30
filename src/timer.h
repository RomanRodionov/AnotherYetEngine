#pragma once
#include "common.h"

class Timer
{
    double lastTime;
    float delta;
public:
    Timer() {}
    void init()
    {
        lastTime = glfwGetTime();
        delta = 0.f;
    }
    void update()
    {
        double curTime = glfwGetTime();
        delta = float(curTime - lastTime);
        lastTime = curTime;
    }
    double get_delta() const
    {
        return delta;
    }
    double get_time() const
    {
        return glfwGetTime();
    }
};

class FPSCounter
{
    double lastTime;
    uint frames;
    Timer *timer;
    float period;
    float max_delta;
public:
    FPSCounter(Timer *t, float p=1.f) : timer(t), period(p) {}
    void init()
    {
        lastTime = timer->get_time();
        frames = 0;
        max_delta = -1.f;
    }
    void update()
    {
        frames++;
        float delta = timer->get_delta();
        if (delta > max_delta)
        {
            max_delta = delta;
        }
    }
    bool ready()
    {
        return (timer->get_time() - lastTime) > period;
    }
    void print_fps()
    {
        float t = timer->get_time() - lastTime;
        std::cout << "FPS: " << frames / t << " AvgTime: " << 1000.f * t / static_cast<float>(frames) << " ms " << "MaxTime: " << max_delta * 1000.f << " ms" << std::endl;
    }
    void print_fps(std::string& str)
    {
        std::stringstream ss;
        float t = timer->get_time() - lastTime;
        ss << "FPS: " << frames / t << "\nAvgTime: " << 1000.f * t / static_cast<float>(frames) << " ms" << "\nMaxTime: " << max_delta * 1000.f << " ms" << std::endl;
        str = ss.str();
    }
};