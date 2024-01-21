
#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"

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

Texture2D::Texture2D(const Img& img, TexType type)
{
    m_type = type;
    GLint pixel_mode;
    GLint internal_mode; 
    switch(img.ch)
    {
        case 1:
            pixel_mode = GL_RED;
            internal_mode = GL_RED;
            break;
        case 2:
            pixel_mode = GL_RG;
            internal_mode = GL_RG;
            break;
        case 3:
            pixel_mode = GL_RGB;
            internal_mode = GL_RGB;
            break;
        case 4:
            pixel_mode = GL_RGBA;
            internal_mode = GL_RGBA;
            break;
    }
    glGenTextures(1, &m_textureID);

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_mode, img.w, img.h, 0, pixel_mode, GL_UNSIGNED_BYTE, img.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(const aiTexture* tex, TexType type)
{
    m_type = type;
    GLint pixel_mode;
    GLint internal_mode; 
    pixel_mode = GL_RGBA;
    internal_mode = GL_BGRA;
    glGenTextures(1, &m_textureID);

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_mode, tex->mWidth, tex->mHeight, 0, pixel_mode, GL_UNSIGNED_BYTE, tex->pcData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}