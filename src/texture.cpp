
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

Texture2D::Texture2D(const Img& img, bool reversed_channels)
{
    GLint pixel_mode    = GL_RGB;
    GLint internal_mode = GL_RGB; 
    if (reversed_channels)
    {
        pixel_mode = GL_BGR;
    }
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_mode, img.w, img.h, 0, pixel_mode, GL_UNSIGNED_BYTE, img.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &textureID);
}