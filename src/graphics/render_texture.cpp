#include "render_texture.h"

#include <glad/glad.h>
#include <iostream>

#include "core.h"

namespace Fusion
{
    RenderTexture::RenderTexture(int width, int height)
    {
        Load(width, height);
    }

    void RenderTexture::Load(int width, int height)
    {
        Unload();
        // Cria o Framebuffer Object
        glGenFramebuffers(1, &m_fboId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);

        // Cria a textura de cor vazia e a anexa ao FBO
        m_Texture = std::make_unique<Texture>(width, height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->GetId(), 0);

        // Cria o Renderbuffer Object para profundidade e estêncil
        glGenRenderbuffers(1, &m_rboId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_rboId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rboId);

        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) // Verifica se o FBO está completo
        {
            std::cerr << "ERRO::FRAMEBUFFER:: Framebuffer não está completo!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Desvincula o FBO para não alterar o framebuffer padrão acidentalmente
    }

    RenderTexture::~RenderTexture()
    {
        Unload();
    }
    void RenderTexture::Unload()
    {
        if (m_fboId != 0 || m_rboId != 0)
        {
            m_Texture->Unload();
            glDeleteFramebuffers(1, &m_fboId);
            glDeleteRenderbuffers(1, &m_rboId);
            m_fboId = 0;
            m_rboId = 0;
        }
    }
} // namespace Fusion