#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
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

        // Decide GL+GLSL versions
        #if defined(IMGUI_IMPL_OPENGL_ES2)
            // GL ES 2.0 + GLSL 100
            const char* glsl_version = "#version 100";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        #elif defined(__APPLE__)
            // GL 3.2 + GLSL 150
            const char* glsl_version = "#version 150";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
        #else
            // GL 3.0 + GLSL 130
            const char* glsl_version = "#version 130";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
            //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
        #endif

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

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(window, true);
        #ifdef __EMSCRIPTEN__
            ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
        #endif
            ImGui_ImplOpenGL3_Init(glsl_version);
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
    void setKeyCallback(GLFWkeyfun callback)
    {
        glfwSetKeyCallback(window, callback);
    }
};
