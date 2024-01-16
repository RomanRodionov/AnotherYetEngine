#pragma once

#include "common.h"

void error_callback(int error, const char *msg);

void initGLFW();
void initGLEW();

class Window
{
private:
    GLFWwindow* window;
    int width, height;
public:
    Window(int width_, int height_, const char *title = "TinyEngine", GLFWmonitor *monitor = NULL, GLFWwindow *share = NULL)
    {
        initGLFW();

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        width = width_;
        height = height_;

        window = glfwCreateWindow(width, height, title, monitor, share);
        setContext();

        if (window == NULL)
        {
            std::cerr << "Failed to open GFLW window" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Failed to open GFLW window");
        }

        initGLEW();
    }
    void setContext()
    {
        glfwMakeContextCurrent(window);
    }
    void setInputMode(int mode = GLFW_STICKY_KEYS, int value = GL_TRUE)
    {
        glfwSetInputMode(window, mode, value);
    }
    void setSwapInterval(int interval = 1) // vertical sychronization
    {
        glfwSwapInterval(interval); 
    }
    void swapBuffers()
    {
        glfwSwapBuffers(window);
    }
    int shouldClose()
    {
        return glfwWindowShouldClose(window);
    }
    void getRes(int *w, int *h)
    {
        *w = width;
        *h = height;
    }
    //controls
    void pollEvents()
    {
        glfwPollEvents();
    }
    void setCursorPos(double xpos, double ypos)
    {
        glfwSetCursorPos(window, xpos, ypos);
    }
    void getCursorPos(double *xpos, double *ypos)
    {
        glfwGetCursorPos(window, xpos, ypos);
    }
    int getKey(int key)
    {
        return glfwGetKey(window, key);
    }
};
