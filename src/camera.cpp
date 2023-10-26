#include "common.h"
#include "camera.h"

static double scroll_offset_x = 0, scroll_offset_y = 0;

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    scroll_offset_x += xoffset;
    scroll_offset_y += yoffset;
}

glm::mat4 FPCamera::create_mvp_matrix()
{
    glm::mat4 proj = glm::perspective(glm::radians(FoV), (float) WIDTH / (float) HEIGHT, 0.1f, 100.f);
    glm::mat4 view = glm::lookAt(
        position, // camera's position
        position + direction,  // focus direction
        up   // up direction
    );
    glm::mat4 model = glm::mat4(1.f);
    return proj * view * model;
}

void FPCamera::update_controls(GLFWwindow *window, float delta)
{
// mouse
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

    horizontalAngle += mouseSpeed * delta * float(WIDTH / 2 - xpos);
    verticalAngle += mouseSpeed * delta * float(HEIGHT / 2 - ypos);
    verticalAngle = clamp(verticalAngle, -PI / 2.f, PI / 2.f);

    direction = glm::vec3(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    glm::vec3 right = glm::cross(direction, up);

    FoV = initialFoV - 5 * scroll_offset_y;

// keyboard

    direction = glm::normalize(direction);
    right = glm::normalize(right);
    up = glm::normalize(up);

    glm::vec3 move(0);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        move += direction;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        move -= direction;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        move += right;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        move -= right;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        move += up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        move -= up;
    }
    // return initial settings
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        init();
        move = glm::vec3(0);
    }
    if (glm::length(move) > 0.00001)
    {
        position += glm::normalize(move) * delta * speed;
    }
}