#include "texture.h"

#include <glad/glad.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "core.h"
#include "stb_image.h"

namespace Fusion
{
    Texture::Texture(const char* path)
    {
        LoadFromFile(path);
    }

    Texture::Texture(unsigned char* data, size_t len)
    {
        LoadFromMemory(data, len);
    }

    Texture::Texture(int width, int height)
        : m_width(width), m_height(height), m_nrChannels(4)
    {

        m_isFboTexture = true;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        // Aloca memória na GPU para a textura, mas não envia dados (passa NULL)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Desvincula para segurança
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::Texture()
    {
    }

    Texture::~Texture()
    {
        Unload();
    }

    void Texture::SetSmooth(bool enable)
    {
        glBindTexture(GL_TEXTURE_2D, m_id);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (enable) ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (enable) ? GL_LINEAR : GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    bool Texture::Load(unsigned char* data)
    {
        glDeleteTextures(1, &m_id);
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        // Evita problemas de alinhamento em RGB
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (data)
        {
            GLenum format = (m_nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Failed to load texture\n";
            return false;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        return true;
    }

    bool Texture::LoadFromFile(const char* path)
    {
        if (m_isFboTexture)
        {
            std::cout << "Failed to load texture this is FBO Texture\n";
            return false;
        }
        unsigned char* data = stbi_load(path, &m_width, &m_height, &m_nrChannels, 0);
        const bool loadedData = Load(data);
        stbi_image_free(data);
        return loadedData;
    }

    bool Texture::LoadFromMemory(unsigned char* buffer, size_t len)
    {
        if (m_isFboTexture)
        {
            std::cout << "Failed to load texture this is FBO Texture\n";
            return false;
        }
        unsigned char* data = stbi_load_from_memory(buffer, len, &m_width, &m_height, &m_nrChannels, 0);
        const bool loadedData = Load(data);
        stbi_image_free(data);
        return loadedData;
    }

    Sizei Texture::GetSize() const
    {
        return Sizei{m_width, m_height};
    }

    unsigned int Texture::GetId() const
    {
        return m_id;
    }

    bool Texture::IsFboTexture() const
    {
        return m_isFboTexture;
    }

    void Texture::Unload()
    {
        if (m_id != 0 && Core::HasWindowActive())
        {
            glDeleteTextures(1, &m_id);
            m_id = 0; // Evita dupla liberação
        }
    }

} // namespace Fusion
