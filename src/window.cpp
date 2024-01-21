#include "window.h"

void error_callback(int error, const char *msg) {
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    std::cerr << s << std::endl;
}

void initGLFW()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

void initGLEW()
{
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        throw std::runtime_error("Failed to initialize GLEW");
    }
}