#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstddef> // size_t

#include "fusion_math.h"

namespace Fusion
{

    class Texture
    {
    public:

        Texture();
        ~Texture();
        Texture(const char* path);
        Texture(int width, int height);
        Texture(const Texture&) = delete; // Desabilita a cópia
        Texture& operator=(const Texture&) = delete;
        Texture(unsigned char* data, size_t len);
        bool LoadFromFile(const char* path);
        bool LoadFromMemory(unsigned char* data, size_t len);
        unsigned int GetId() const;
        void SetSmooth(bool enable);
        Sizei GetSize() const;
        bool IsFboTexture() const;
        void Unload();

    private:

        unsigned int m_Id = 0;
        int m_Width = 0;
        int m_Height = 0;
        int m_NrChannels = 0;
        bool m_IsFboTexture = false;
        bool Load(unsigned char* data);
    };

} // namespace Fusion

#endif