#pragma once

#include "common.h"

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

class FPCamera
{
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    float horizontalAngle;
    float verticalAngle;
    float initialFoV;
    float FoV;
    float speed;
    float mouseSpeed;
public:
    FPCamera(GLFWwindow *window)
    {
        init();
        glfwSetScrollCallback(window, scroll_callback);
    }
    void init()
    {
        position        = glm::vec3(0, 0, 5);
        direction       = glm::vec3(0, 0, 0);
        up              = glm::vec3(0, 1, 0);
        horizontalAngle = PI;
        verticalAngle   = 0.0f;
        initialFoV      = 45.0f;
        FoV             = initialFoV;
        speed           = 6.0f;
        mouseSpeed      = 0.05f;
    }
    glm::mat4 create_mvp_matrix();
    void update_controls(GLFWwindow *window, float delta);
};