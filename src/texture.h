#pragma once

#include "../include/stb/stb_image.h"

#include "common.h"
#include <cstdlib> 

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
    Img(const char* path, int channels=-1)
    {
        if (channels == -1)
        {
            stbi_info(path, &w, &h, &channels);
        }
        data = stbi_load(path, &w, &h, &ch, channels);
        if(data == NULL) {
            std::cerr << "Unable to load image " << path << std::endl;
        }
        ch = channels;
        mode = stbi;
    }
    ~Img();
};

enum class TexType : uint
{
    NONE,
    DIFFUSE,
    SPECULAR,
    AMBIENT,
    EMISSIVE,
    HEIGHT,
    NORMALS,
    SHININESS,
    OPACITY,
    DISPLACEMENT,
    LIGHTMAP,
    SIZE
};

class Texture2D
{
private:
    GLuint m_textureID;
    TexType m_type;
public:
    Texture2D(const Img& img, TexType type=TexType::NONE);
    Texture2D(const char* path, TexType type=TexType::NONE) : Texture2D(Img(path), type) {}
    Texture2D(const aiTexture* tex, TexType type=TexType::NONE);
    ~Texture2D() {}
    void bind()
    {
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }
    void unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    TexType getType()
    {
        return m_type;
    }
    void deleteTexture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &m_textureID);
    }
};