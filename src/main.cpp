#include "common.h"
#include "camera.h"
#include "texture.h"
#include "load_model.h"
#include "timer.h"
#include "renderable.h"
#include <iostream>

int main()
{
    Timer timer;
    FPSCounter fps_counter(&timer);
    float delta = 0.f;

    GLFWwindow *window = create_glfw_window();
    if (!window)
    {
        return -1;
    }

    FPCamera camera(window);
    glm::ivec2 resolution = glm::ivec2(WIDTH, HEIGHT);
    

    VertFragShader passShader = VertFragShader(PATH("shaders/pass_vert.glsl"), PATH("shaders/pass_frag.glsl"));
    passShader.use();
    passShader.setInt("tex", 0);
    ComputeShader rtShader = ComputeShader(PATH("shaders/ray_tracing_comp.glsl"));
    rtShader.use();
    rtShader.setIvec2("resolution", resolution);

    GLuint renderTex;
    glGenTextures(1, &renderTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, renderTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    timer.init();
    fps_counter.init();

    unsigned int quadVAO;
    unsigned int quadVBO;
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


    do //main loop
    {
// graphics
        //glViewport(0, 0, WIDTH, HEIGHT);

        rtShader.use();
        rtShader.setVec3("position", camera.position);
        rtShader.setVec3("horizontal", camera.horizontal);
        rtShader.setVec3("vertical", camera.vertical);
        rtShader.setVec3("ll_corner", camera.ll_corner);
        glDispatchCompute(WIDTH / 16, HEIGHT / 16, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        passShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderTex);
        glBindVertexArray(quadVAO);
	    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	    glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

// physics and controls
        timer.update();
        delta = timer.get_delta();

        fps_counter.update();
        if (fps_counter.ready())
        {
            fps_counter.print_fps();
            fps_counter.init();
        }

        camera.update_controls(window, delta);

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);
	glDeleteTextures(1, &renderTex);
    glfwTerminate();

    return 0;
}