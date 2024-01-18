#pragma once

#include "../include/stb/stb_image.h"

#include "common.h"
#include <cstdlib> 

class Img
{
private:
    enum LoadMode
    {
        stbi,
        c_malloc,
        cpp_new
    };
public:
    int w, h, ch;
    uchar* data;
    LoadMode mode;
    Img(const char* path, int channels=3)
    {
        data = stbi_load(path, &w, &h, &ch, channels);
        mode = stbi;
    }
    ~Img();
};

class Texture2D
{
private:
    GLuint textureID;
public:
    Texture2D(const Img& img, bool reversed_channels=false);
    Texture2D(const char* path, bool reversed_channels=false) : Texture2D(Img(path), reversed_channels) {}
    ~Texture2D();
    void bind()
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    void unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};