#include "common.h"
#include "shapes.h"
#include "camera.h"
#include "texture.h"
#include "load_model.h"
#include "timer.h"
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

    GLuint programID = LoadShaders(PATH("shaders/vert.glsl"),
                                     PATH("shaders/frag.glsl"));

    GLuint MVP_ID = glGetUniformLocation(programID, "MVP");
    GLuint VIEW_ID = glGetUniformLocation(programID, "view");
    GLuint MODEL_ID = glGetUniformLocation(programID, "model");
    GLuint LIGHT_COLOR_ID = glGetUniformLocation(programID, "lightColor");
    GLuint LIGHT_POWER_ID = glGetUniformLocation(programID, "lightPower");
    GLuint LIGHT_POS_WORLD_ID = glGetUniformLocation(programID, "lightPosWorld");
    GLuint MODEL_VIEW_3x3_ID = glGetUniformLocation(programID, "MV3x3");

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents; 

    bool res = loadOBJ(PATH("data/models/monkey_highpoly.obj"), indices, vertices, uvs, normals);
    computeTangentBasis(indices, vertices, uvs, normals, tangents, bitangents);

    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    GLuint tangentbuffer;
    glGenBuffers(1, &tangentbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
    glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);

    GLuint bitangentbuffer;
    glGenBuffers(1, &bitangentbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
    glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], GL_STATIC_DRAW);

    GLuint diffuseTex = load_texture(PATH("data/materials/wood_floor/BaseColor.jpg"), false);
    GLuint diffuseTexID = glGetUniformLocation(programID, "diffuseTexSampler");
    GLuint normalTex = load_texture(PATH("data/materials/wood_floor/Normal.jpg"), false);
    GLuint normalTexID = glGetUniformLocation(programID, "normalTexSampler");
    GLuint roughTex = load_texture(PATH("data/materials/wood_floor/Roughness.jpg"), false);
    GLuint roughTexID = glGetUniformLocation(programID, "roughTexSampler");
    GLuint aoTex = load_texture(PATH("data/materials/wood_floor/AmbientOcclusion.jpg"), false);
    GLuint aoTexID = glGetUniformLocation(programID, "aoTexSampler");

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    timer.init();
    fps_counter.init();

    do //main loop
    {
// graphics
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //bind vertices buffer
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        //bind normal buffer
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        //bind uv buffer
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        //bind tangent buffer
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
        glVertexAttribPointer(
            3,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        //bind bitangent buffer
        glEnableVertexAttribArray(4);
        glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
        glVertexAttribPointer(
            4,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        glm::mat4 mvp = camera.create_mvp_matrix();
        glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &mvp[0][0]);

        glm::mat4 view = camera.create_view_matrix();
        glUniformMatrix4fv(VIEW_ID, 1, GL_FALSE, &view[0][0]);

        glm::mat4 model = glm::mat4(1.f);
        glUniformMatrix4fv(MODEL_ID, 1, GL_FALSE, &model[0][0]);

        glm::mat4 modelView = view * model;
        glm::mat3 modelView3x3 = glm::mat3(modelView);
        glUniformMatrix3fv(MODEL_VIEW_3x3_ID, 1, GL_FALSE, &modelView3x3[0][0]);

        glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
        glUniform3fv(LIGHT_COLOR_ID, 1, &lightColor[0]);

        GLfloat lightPower = 100.f;
        glUniform1f(LIGHT_POWER_ID, lightPower);

        glm::vec3 lightPosWorld = glm::vec3(10.f * sin(timer.get_time()), 10.f, 10.f * cos(timer.get_time()));
        glUniform3fv(LIGHT_POS_WORLD_ID, 1, &lightPosWorld[0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTex);
        glUniform1i(diffuseTexID, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalTex);
        glUniform1i(normalTexID, 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, roughTex);
        glUniform1i(roughTexID, 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, aoTex);
        glUniform1i(aoTexID, 3);

        glUseProgram(programID);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
        //glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);

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

    return 0;
}