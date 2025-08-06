#include "window.h"
#include "platform_desktop_glfw.h"
#include "renderer.h"
#include "sprite.h"
#include "text.h"
#include "render_texture.h"
#include "camera2D.h"
#include <cmath>

namespace Fusion
{

    Window::Window()
    {
    }

    Window::~Window()
    {
        if (m_Platform)
        {
            m_Platform->Shutdown();
        }
    }

    void Window::InitWindow(const char *title, int width, int height)
    {
        m_Render = std::make_unique<Renderer>();
        m_Platform = std::make_unique<PlatformDesktopGLFW>();
        m_Platform->Init(title, width, height);
        m_Render->Init(width, height);

        m_DefaultProjection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);
    }

    bool Window::WindowShouldClose()
    {
        return m_Platform->WindowShouldClose();
    }

    bool Window::IsWindowResize()
    {
        return m_Platform->IsWindowResized();
    }

    Sizei Window::GetWindowSize() const
    {
        return m_Platform->GetWindowSize();
    }

    void Window::Clear(Color color)
    {
        m_Platform->Clear(color);
    }

    void Window::BeginDrawing()
    {
        if (IsWindowResize())
        {
            Sizei newSize = m_Platform->GetWindowSize();
            glm::mat4 newProjection = glm::ortho(0.0f, static_cast<float>(newSize.width), static_cast<float>(newSize.height), 0.0f);
            m_Render->SetProjection(newProjection);
            m_DefaultProjection = newProjection;
        }
        m_Render->BeginRender();
    }

    void Window::Draw(const Sprite &sprite, Shader* customShader)
    {
        m_Render->DrawTexture(sprite, customShader);
    }

    void Window::Draw(Text &text, Shader* customShader)
    {
        m_Render->DrawText(text, customShader);
    }

    void Window::EndDrawing()
    {
        m_Render->EndRender();
        m_Platform->SwapBuffersPollEvents();
    }

    void Window::BeginScissorMode(int x, int y, int width, int height)
    {
        // 1. Desenha tudo que estava no lote atual ANTES de ativar o Scissor.
        m_Render->EndRender();

        // 2. Converte a coordenada Y do sistema "top-left" para "bottom-left" do OpenGL.
        const int openglY = GetWindowSize().height - (y + height);

        // 3. Ativa o teste Scissor com as coordenadas corretas.
        m_Render->BeginScissorMode(x, openglY, width, height);
    }

    void Window::EndScissorMode()
    {
        // 1. Desenha tudo que estava no lote DENTRO do modo Scissor.
        m_Render->EndRender();
        m_Render->EndScissorMode();
    }

    void Window::BeginTextureMode(const RenderTexture &target)
    {
        m_Render->EndRender(); // Flush antes de mudar o alvo

        glBindFramebuffer(GL_FRAMEBUFFER, target.GetFboId());

        const auto size = target.GetTexture()->GetSize();
        glViewport(0, 0, size.width, size.height);

        // Define uma nova projeção ortográfica para o tamanho da textura
        glm::mat4 textureProjection = glm::ortho(0.0f, (float)size.width, (float)size.height, 0.0f);
        m_Render->SetProjection(textureProjection);

        // Limpa a textura com uma cor (opcional, mas recomendado)
        // Usamos m_Platform->Clear para manter a abstração
        m_Platform->Clear({0.0f, 0.0f, 0.0f, 0.0f});
    }

    void Window::EndTextureMode()
    {
        m_Render->EndRender(); // Flush no que foi desenhado na textura

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Volta para a tela

        const Sizei size = GetWindowSize();
        glViewport(0, 0, size.width, size.height);
        m_Render->SetProjection(m_DefaultProjection); // Restaura a projeção da janela
    }

    void Window::BeginMode2D(const Camera2D &camera)
    {
        m_Render->EndRender(); // Desenha qualquer coisa pendente no modo padrão
        m_Render->SetViewMatrix(camera.GetViewMatrix());
    }
    void Window::EndMode2D()
    {
        m_Render->EndRender(); // Desenha qualquer coisa pendente no modo 2D
        m_Render->ResetViewMatrix();
    }
    double Window::GetTime() const
    {
        return glfwGetTime();
    }
}
