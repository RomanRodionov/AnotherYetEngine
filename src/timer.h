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
public:
    FPSCounter(Timer *t, float p=1.f) : timer(t), period(p) {}
    void init()
    {
        lastTime = timer->get_time();
        frames = 0;
    }
    void update()
    {
        frames++;
    }
    bool ready()
    {
        return (timer->get_time() - lastTime) > period;
    }
    void print_fps()
    {
        float t = timer->get_time() - lastTime;
        std::cout << "FPS: " << frames / t << std::endl;
    }
};