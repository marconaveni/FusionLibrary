#ifndef TEXTURE_H
#define TEXTURE_H

#include "fusion_math.h"

namespace Fusion
{

    class Texture
    {
    public:
        // Desabilita a cópia
        Texture(const Texture &) = delete;
        Texture &operator=(const Texture &) = delete;
        Texture(const char *path);
        Texture(int width, int height);
        ~Texture();
        bool LoadFromFile(const char *path);
        Sizei GetSize() const;
        unsigned int GetId() const;
        bool IsFboTexture() const;

    private:
        unsigned int m_Id = 0;
        int m_Width = 0;
        int m_Height = 0;
        int m_NrChannels = 0;
        bool m_IsFboTexture = false;
    };

}

#endif