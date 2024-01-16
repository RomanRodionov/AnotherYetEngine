#include "common.h"
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

    Window window(WIDTH, HEIGHT);

    FPCamera camera;

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

    GLuint diffuseTex = load_texture(PATH("data/materials/art_deco/BaseColor.jpg"), false);
    GLuint diffuseTexID = glGetUniformLocation(programID, "diffuseTexSampler");
    GLuint normalTex = load_texture(PATH("data/materials/art_deco/Normal.jpg"), false);
    GLuint normalTexID = glGetUniformLocation(programID, "normalTexSampler");
    GLuint roughTex = load_texture(PATH("data/materials/art_deco/Roughness.jpg"), false);
    GLuint roughTexID = glGetUniformLocation(programID, "roughTexSampler");
    GLuint aoTex = load_texture(PATH("data/materials/art_deco/AmbientOcclusion.jpg"), false);
    GLuint aoTexID = glGetUniformLocation(programID, "aoTexSampler");

//framebuffer stuff
    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    GLuint renderTex;
    glGenTextures(1, &renderTex);

    glBindTexture(GL_TEXTURE_2D, renderTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTex, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        //std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << " " << GL_FRAMEBUFFER_COMPLETE << std::endl;
        return false;
    }

    std::cout << "BBB" << std::endl;

    GLuint quad_VertexArrayID;
    glGenVertexArrays(1, &quad_VertexArrayID);
    glBindVertexArray(quad_VertexArrayID);

    static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
    };

    GLuint quad_vertexbuffer;
    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
    GLuint quad_programID = LoadShaders(PATH("shaders/pass_vert.glsl"), PATH("shaders/simple_tex_frag.glsl"));
    GLuint texID = glGetUniformLocation(quad_programID, "renderTex");
    GLuint timeID = glGetUniformLocation(quad_programID, "time");    

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    timer.init();
    fps_counter.init();

    do //main loop
    {
// graphics
        glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
        glViewport(0, 0, WIDTH, HEIGHT);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

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

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
        //glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);

// render to the screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(quad_programID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderTex);
        glUniform1i(texID, 0);
        glUniform1f(timeID, (float)timer.get_time() * 10.f);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);

        window.swapBuffers();
        window.pollEvents();

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

    } while (window.getKey(GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             window.shouldClose() == 0);

    glDeleteBuffers(1, &elementbuffer);
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &diffuseTex);
    glDeleteTextures(1, &normalTex);
    glDeleteTextures(1, &aoTex);
    glDeleteTextures(1, &roughTex);

    glDeleteFramebuffers(1, &FramebufferName);
	glDeleteTextures(1, &renderTex);
	glDeleteRenderbuffers(1, &depthrenderbuffer);
	glDeleteBuffers(1, &quad_vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);

    glfwTerminate();

    return 0;
}