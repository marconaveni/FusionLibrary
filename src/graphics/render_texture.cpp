#include "render_texture.h"

#include <glad/glad.h>
#include <iostream>

namespace Fusion
{
    // Precisaremos de um novo construtor na classe Texture para criar uma textura vazia
    // Vamos assumir que ele existe por enquanto.
    RenderTexture::RenderTexture(int width, int height)
    {
        // 1. Cria o Framebuffer Object
        glGenFramebuffers(1, &m_FboId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);

        // 2. Cria a textura de cor vazia e a anexa ao FBO
        m_Texture = std::make_unique<Texture>(width, height); // Novo construtor necessário!
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->GetId(), 0);

        // 3. Cria o Renderbuffer Object para profundidade e estêncil
        glGenRenderbuffers(1, &m_RboId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RboId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RboId);

        // 4. Verifica se o FBO está completo
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "ERRO::FRAMEBUFFER:: Framebuffer não está completo!" << std::endl;
        }

        // 5. Desvincula o FBO para não alterar o framebuffer padrão acidentalmente
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    RenderTexture::~RenderTexture()
    {
        glDeleteFramebuffers(1, &m_FboId);
        glDeleteRenderbuffers(1, &m_RboId);
        // O m_Texture é liberado automaticamente pelo unique_ptr
    }
} // namespace Fusion