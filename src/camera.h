#pragma once

#include "common.h"
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
    FPCamera()
    {
        init();
    }
    void init()
    {
        position        = glm::vec3(0, 0, 5);
        up              = glm::vec3(0, 1, 0);
        horizontalAngle = PI;
        verticalAngle   = 0.0f;
        initialFoV      = 45.0f;
        FoV             = initialFoV;
        speed           = 3.0f;
        mouseSpeed      = 0.001f;

        direction = glm::vec3(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );
    }
    glm::mat4 create_mvp_matrix();
    glm::mat4 create_view_matrix();
    void update_controls(Window& window, float delta);
};