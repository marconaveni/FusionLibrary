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

    Texture::Texture(unsigned char *data, size_t len)
    {
        LoadFromMemory(data, len);
    }

    Texture::Texture(int width, int height)
        : m_Width(width), m_Height(height), m_NrChannels(4)
    {

        m_IsFboTexture = true;

        glGenTextures(1, &m_Id);
        glBindTexture(GL_TEXTURE_2D, m_Id);

        // Aloca memória na GPU para a textura, mas não envia dados (passa NULL)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Desvincula para segurança
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_Id);
    }

    void Texture::SetSmooth(bool enable)
    {
        glBindTexture(GL_TEXTURE_2D, m_Id);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (enable) ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (enable) ? GL_LINEAR : GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    bool Texture::Load(unsigned char *data)
    {
        glDeleteTextures(1, &m_Id);
        glGenTextures(1, &m_Id);
        glBindTexture(GL_TEXTURE_2D, m_Id);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (data)
        {
            GLenum format = (m_NrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Failed to load texture\n";
            return false;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        return true;
    }

    bool Texture::LoadFromFile(const char *path)
    {
        if (m_IsFboTexture)
        {
            std::cout << "Failed to load texture this is FBO Texture\n";
            return false;
        }
        unsigned char *data = stbi_load(path, &m_Width, &m_Height, &m_NrChannels, 0);
        Load(data);
        stbi_image_free(data);
        return true;
    }

    bool Texture::LoadFromMemory(unsigned char *buffer, size_t len)
    {
        if (m_IsFboTexture)
        {
            std::cout << "Failed to load texture this is FBO Texture\n";
            return false;
        }
        unsigned char *data = stbi_load_from_memory(buffer, len, &m_Width, &m_Height, &m_NrChannels, 0);
        Load(data);
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

    bool Texture::IsFboTexture() const
    {
        return m_IsFboTexture;
    }

} // namespace Fusion
