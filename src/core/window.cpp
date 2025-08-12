#include "window.h"

#if defined(FUSION_PLATFORM_WEB)
#include "platform_web.h"
#else
#include "platform_desktop_glfw.h"
#endif

#include "renderer.h"
#include "sprite.h"
#include "text.h"
#include "render_texture.h"
#include "camera2d.h"
#include "font_data.h"
#include "core.h"
#include <cmath>

namespace Fusion
{

    Window::Window()
    {
    }

    Window::~Window()
    {
        Close();
    }

    void Window::InitWindow(const char *title, int width, int height)
    {
        Core::Init();
        Core::RegisterWindow();

        m_Render = std::make_unique<Renderer>();

#if defined(FUSION_PLATFORM_WEB)
        // Se estivermos compilando para a web, usa a PlatformWeb
        m_Platform = std::make_unique<PlatformWeb>();
#else
        // Senão, usa a plataforma de desktop padrão
        m_Platform = std::make_unique<PlatformDesktopGLFW>();
#endif

        m_Platform->Init(title, width, height);
        m_Render->Init(width, height);

        m_DefaultProjection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);

        m_defaultFont.LoadFromMemory(DefaultFont::NataSansRegular, DefaultFont::NataSansRegularLen, 32);
    }

    void Window::Close()
    {
        if (m_Platform->IsWindowActive())
        {
            // a ordem importa descarregamos os recursos de font e render antes de fechar o janela/contexto do opengl
            m_defaultFont.Unload();
            m_Render->Shutdown();
            m_Platform->Shutdown();
            Core::UnregisterWindow();
        }
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
        m_Platform->MakeContextCurrent();
        if (IsWindowResize())
        {
            Sizei newSize = m_Platform->GetWindowSize();
            glm::mat4 newProjection = glm::ortho(0.0f, static_cast<float>(newSize.width), static_cast<float>(newSize.height), 0.0f);
            m_Render->SetProjection(newProjection);
            m_DefaultProjection = newProjection;
        }
        m_Render->BeginRender();
    }

    void Window::Draw(const Sprite &sprite, Shader *customShader)
    {
        m_Render->DrawTexture(sprite, customShader);
    }

    void Window::Draw(Text &text, Shader *customShader)
    {
        m_Render->DrawText(text, customShader);
    }

    void Window::DrawRectangle(int x, int y, int width, int height, Color color)
    {
        m_Render->DrawRectangle(x, y, width, height, color);
    }

    void Window::DrawCircle(int centerX, int centerY, float radius, Color color)
    {
        m_Render->DrawCircle(Vector2f({(float)centerX, (float)centerY}), radius, color);
    }

    void Window::DrawCircle(Vector2f center, float radius, Color color)
    {
        m_Render->DrawCircle(center, radius, color);
    }

    void Window::DrawCircleLines(int centerX, int centerY, float radius, Color color)
    {
        m_Render->DrawCircleLines(Vector2f({(float)centerX, (float)centerY}), radius, color);
    }

    void Window::DrawCircleLines(Vector2f center, float radius, Color color)
    {
        m_Render->DrawCircleLines(center, radius, color);
    }

    void Window::DrawTriangle(Vector2f v1, Vector2f v2, Vector2f v3, Color color)
    {
        m_Render->DrawTriangle(v1, v2, v3, color);
    }

    void Window::DrawLine(Vector2f startPos, Vector2f endPos, float thick, Color color)
    {
        m_Render->DrawLine(startPos, endPos, thick, color);
    }

    void Window::DrawRectangleLines(int x, int y, int width, int height, float thick, Color color)
    {
        m_Render->DrawRectangleLines(x, y, width, height, thick, color);
    }

    void Window::EndDrawing()
    {
        m_Render->EndRender();
        m_Platform->PollEventsAndUpdate();
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

    void Window::BeginBlendMode(BlendMode mode)
    {
        m_Render->BeginBlendMode(mode);
    }

    void Window::EndBlendMode()
    {
        m_Render->EndBlendMode();
    }

    float Window::GetFrameTime() const
    {
        return m_Platform->GetFrameTime();
    }

    int Window::GetFPS() const
    {
        return m_Platform->GetFPS();
    }

    double Window::GetTime() const
    {
        return m_Platform->GetTime();
    }

    void Window::SetTargetFPS(int fps)
    {
        m_Platform->SetTargetFPS(fps);
    }

    // --- IMPLEMENTAÇÃO DOS MÉTODOS DE INPUT ---
    bool Window::IsKeyPressed(int key) const { return m_Platform->IsKeyPressed(key); }
    bool Window::IsKeyDown(int key) const { return m_Platform->IsKeyDown(key); }
    bool Window::IsKeyReleased(int key) const { return m_Platform->IsKeyReleased(key); }
    bool Window::IsMouseButtonPressed(int button) const { return m_Platform->IsMouseButtonPressed(button); }
    bool Window::IsMouseButtonDown(int button) const { return m_Platform->IsMouseButtonDown(button); }
    bool Window::IsMouseButtonReleased(int button) const { return m_Platform->IsMouseButtonReleased(button); }
    Vector2f Window::GetMousePosition() const { return m_Platform->GetMousePosition(); }
    float Window::GetMouseWheelMove() const { return m_Platform->GetMouseWheelMove(); }

    // --- IMPLEMENTAÇÃO DOS MÉTODOS DE GAMEPAD ---
    bool Window::IsGamepadAvailable(int gamepad) const { return m_Platform->IsGamepadAvailable(gamepad); }
    const char *Window::GetGamepadName(int gamepad) const { return m_Platform->GetGamepadName(gamepad); }
    bool Window::IsGamepadButtonPressed(int gamepad, int button) const { return m_Platform->IsGamepadButtonPressed(gamepad, button); }
    bool Window::IsGamepadButtonDown(int gamepad, int button) const { return m_Platform->IsGamepadButtonDown(gamepad, button); }
    bool Window::IsGamepadButtonReleased(int gamepad, int button) const { return m_Platform->IsGamepadButtonReleased(gamepad, button); }
    float Window::GetGamepadAxisMovement(int gamepad, int axis) const { return m_Platform->GetGamepadAxisMovement(gamepad, axis); }

    Font &Window::GetDefaultFont()
    {
        return m_defaultFont;
    }

    void Window::SetMainLoop(std::function<void()> loop)
    {
        // A Window tem a definição completa, então ela pode chamar o método.
        m_Platform->SetMainLoop(std::move(loop));
    }

}
