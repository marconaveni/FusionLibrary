#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "texture.h"
#include <memory>

namespace Fusion
{
    class RenderTexture
    {
    public:
        // Desabilita cópia para evitar problemas com IDs do OpenGL
        RenderTexture(const RenderTexture&) = delete;
        RenderTexture& operator=(const RenderTexture&) = delete;

        RenderTexture(int width, int height);
        ~RenderTexture();

        unsigned int GetFboId() const { return m_FboId; }
        const Texture* GetTexture() const { return m_Texture.get(); }

    private:
        unsigned int m_FboId = 0; // ID do Framebuffer Object
        unsigned int m_RboId = 0; // ID do Renderbuffer Object (para profundidade)
        std::unique_ptr<Texture> m_Texture;
    };
}

#endif