#include "main.h"
#include "common.h"
#include "shapes.h"
#include "camera.h"
#include "texture.h"

class Timer
{
    double lastTime;
    float delta;
public:
    Timer() {}
    void init()
    {
        lastTime = glfwGetTime();
        delta = 0.f;
    }
    void update()
    {
        double curTime = glfwGetTime();
        delta = float(curTime - lastTime);
        lastTime = curTime;
    }
    double get_delta()
    {
        return delta;
    }
};

int main()
{
    Timer timer;
    float delta = 0.f;

    GLFWwindow *window = create_glfw_window();
    if (!window)
    {
        return -1;
    }

    FPCamera camera(window);

    GLuint programID = LoadShaders(PATH("shaders/vert.glsl"),
                                     PATH("shaders/frag.glsl"));

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    static const GLfloat g_vertex_buffer_data[] = 
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };

    static const GLfloat g_color_buffer_data[] =
    {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f
    };

    static const GLfloat g_uv_buffer_data[] =
    {
        0.f, 1.f,
        0.f, 1.f,
        0.f, 1.f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_buffer), cube_buffer, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color_buffer), cube_color_buffer, GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_uv_buffer), cube_uv_buffer, GL_STATIC_DRAW);

    GLuint texId = load_texture(PATH("data/textures/brick.png"));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    timer.init();

    int counter = 0;

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

        //bind color buffer
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
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


        glm::mat4 mvp = camera.create_mvp_matrix();

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        glBindTexture(GL_TEXTURE_2D, texId);

        glUseProgram(programID);
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

// physics and controls
        timer.update();
        delta = timer.get_delta();

        if ((counter++) % 100 == 0)
        {
            std::cout << 1 / delta << std::endl;
        }

        camera.update_controls(window, delta);

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    return 0;
}