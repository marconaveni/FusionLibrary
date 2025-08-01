#include "texture.h"
#include <glad/glad.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Fusion
{
    Texture::Texture(const char *path)
    {
        LoadFromFile(path);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_Id);
    }

    bool Texture::LoadFromFile(const char *path)
    {
        glDeleteTextures(1, &m_Id);
        glGenTextures(1, &m_Id);
        glBindTexture(GL_TEXTURE_2D, m_Id);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        unsigned char *data = stbi_load(path, &m_Width, &m_Height, &m_NrChannels, 0);
        if (data)
        {
            GLenum format = (m_NrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
            // glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
            return false;
        }
        stbi_image_free(data);
        return true;
    }

    Sizei Texture::GetSize() const
    {
        return Sizei{m_Width, m_Height};
    }

    unsigned int Texture::GetId() const
    {
        return m_Id;
    }

} // namespace Fusion
