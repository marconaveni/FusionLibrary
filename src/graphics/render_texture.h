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

        RenderTexture() = default;
        void Load(int width, int height);
        ~RenderTexture();

        unsigned int GetFboId() const { return m_fboId; }
        const Texture* GetTexture() const { return m_Texture.get(); }

    private:
        unsigned int m_fboId = 0; // ID do Framebuffer Object
        unsigned int m_rboId = 0; // ID do Renderbuffer Object (para profundidade)
        std::unique_ptr<Texture> m_Texture;
    };
}

#endif