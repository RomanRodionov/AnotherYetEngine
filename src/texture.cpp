#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb/stb_image.h"

#include "common.h"
#include "texture.h"
#include <cstdlib>

Img Img::load(const char* path, int channels)
{
    Img t;
    t.data = stbi_load(path, &t.w, &t.h, &t.ch, channels);
    t.mode = stbi;
    return t;
}

Img::~Img()
{
    switch (mode)
    {
    case stbi:
        stbi_image_free(data);
        break;
    case c_malloc:
        free(data);
    case cpp_new:
        delete(data);
    default:
        break;
    }
}

GLuint load_texture(const char* path)
{
    Img img = Img::load(path);
    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.w, img.h, 0, GL_BGR, GL_UNSIGNED_BYTE, img.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return textureID;
}