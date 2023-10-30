#include "main.h"
#include "common.h"
#include "shapes.h"
#include "camera.h"
#include "texture.h"
#include "load_model.h"
#include "timer.h"

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

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool res = loadOBJ(PATH("data/models/monkey.obj"), indices, vertices, uvs, normals);

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

    GLuint texId = load_texture(PATH("data/textures/brick.png"));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

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

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        glm::mat4 mvp = camera.create_mvp_matrix();
        glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &mvp[0][0]);

        glm::mat4 view = camera.create_view_matrix();
        glUniformMatrix4fv(VIEW_ID, 1, GL_FALSE, &view[0][0]);

        glm::mat4 model = glm::mat4(1.f);
        glUniformMatrix4fv(MODEL_ID, 1, GL_FALSE, &model[0][0]);

        glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
        glUniform3fv(LIGHT_COLOR_ID, 1, &lightColor[0]);

        GLfloat lightPower = 100.f;
        glUniform1f(LIGHT_POWER_ID, lightPower);

        glm::vec3 lightPosWorld = glm::vec3(10.f, 10.f, 10.f);
        glUniform3fv(LIGHT_POS_WORLD_ID, 1, &lightPosWorld[0]);

        glBindTexture(GL_TEXTURE_2D, texId);

        glUseProgram(programID);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
        //glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glDisableVertexAttribArray(0);

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