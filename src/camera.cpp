#include "common.h"
#include "camera.h"

static double scroll_offset_x = 0, scroll_offset_y = 0;

glm::mat4 FPCamera::create_mvp_matrix()
{
    glm::mat4 proj = glm::perspective(glm::radians(FoV), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.f);
    glm::mat4 view = glm::lookAt(
        position, // camera's position
        position + direction,  // focus direction
        up   // up direction
    );
    glm::mat4 model = glm::mat4(1.f);
    return proj * view * model;
}

glm::mat4 FPCamera::create_view_matrix()
{
    glm::mat4 view = glm::lookAt(
        position, // camera's position
        position + direction,  // focus direction
        up   // up direction
    );
    return view;
}

void FPCamera::update_controls(Window& window, float delta)
{
// mouse
    double xpos, ypos;
    int width, height;
    window.getRes(&width, &height);
    window.getCursorPos(&xpos, &ypos);
    glm::vec2 offset = {float(width / 2 - xpos), float(height / 2 - ypos)};
    if (length(offset))
    {
        window.setCursorPos(width / 2, height / 2);

        horizontalAngle += mouseSpeed * offset.x;
        verticalAngle += mouseSpeed * offset.y;
        verticalAngle = clamp(verticalAngle, -PI / 2.f, PI / 2.f);

        direction = glm::vec3(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );
    }

    glm::vec3 right = glm::cross(direction, up);

    FoV = initialFoV - 5 * scroll_offset_y;

// keyboard

    direction = glm::normalize(direction);
    right = glm::normalize(right);
    up = glm::normalize(up);

    glm::vec3 move(0);

    if (window.getKey(GLFW_KEY_W) == GLFW_PRESS)
    {
        move += direction;
    }
    if (window.getKey(GLFW_KEY_S) == GLFW_PRESS)
    {
        move -= direction;
    }
    if (window.getKey(GLFW_KEY_D) == GLFW_PRESS)
    {
        move += right;
    }
    if (window.getKey(GLFW_KEY_A) == GLFW_PRESS)
    {
        move -= right;
    }
    if (window.getKey(GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        move += up;
    }
    if (window.getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        move -= up;
    }
    // return initial settings
    if (window.getKey(GLFW_KEY_R) == GLFW_PRESS)
    {
        init();
        move = glm::vec3(0);
    }
    if (glm::length(move) > 0.00001)
    {
        position += glm::normalize(move) * delta * speed;
    }
}