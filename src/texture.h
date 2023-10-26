#pragma once

#define uchar unsigned char

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
    static Img load(const char* path, int channels=3);
    ~Img();
};

GLuint load_texture(const char* path);