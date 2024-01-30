#include "common.h"
#include "camera.h"
#include "texture.h"
#include "load_model.h"
#include "timer.h"
#include "load_shader.h"
#include "model.h"
#include <iostream>

int main()
{
    Timer timer;
    FPSCounter fps_counter(&timer);
    float delta = 0.f;

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT);
    int width, height;
    window.getRes(&width, &height);
    window.setCursorPos(width / 2, height / 2);

    FPCamera camera(&window);

    auto mainShader = VertFragShader(PATH("shaders/diffuse_vert.glsl"),
                                     PATH("shaders/diffuse_frag.glsl"));

/*
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //bool res = loadOBJ(PATH("data/models/monkey_highpoly.obj"), indices, vertices, uvs, normals);
    //computeTangentBasis(indices, vertices, uvs, normals, tangents, bitangents);
*/
    Model model(PATH("data/models/sci-fi-corridor/untitled.fbx"));

/*
//framebuffer stuff
    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    GLuint renderTex;
    glGenTextures(1, &renderTex);

    glBindTexture(GL_TEXTURE_2D, renderTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTex, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        //std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << " " << GL_FRAMEBUFFER_COMPLETE << std::endl;
        return false;
    }

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
    auto postShader = VertFragShader(PATH("shaders/pass_vert.glsl"), PATH("shaders/simple_tex_frag.glsl"));

*/
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    timer.init();
    fps_counter.init();

    do //main loop
    {
// graphics
        //glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        window.getRes(&width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(150, 70));

        bool opened = true;

        ImGui::Begin("ui", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);

        ImGui::SetCursorPos(ImVec2(8, 25));
        std::string message;
        fps_counter.print_fps(message);
        ImGui::Text(message.c_str());

        ImGui::End();
        ImGui::EndFrame();
        ImGui::Render();

        mainShader.use();

        glm::mat4 mvp = camera.create_mvp_matrix();
        mainShader.setMat4("MVP", mvp);

        model.draw(mainShader);
        /*
// render to the screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        postShader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderTex);

        postShader.setInt("renderTex", 0);
        postShader.setFloat("time", (float)timer.get_time() * 10.f);

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
        */

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

    //glDeleteFramebuffers(1, &FramebufferName);
	//glDeleteRenderbuffers(1, &depthrenderbuffer);
	//glDeleteBuffers(1, &quad_vertexbuffer);
	//glDeleteVertexArrays(1, &VertexArrayID);

    glfwTerminate();

    return 0;
}