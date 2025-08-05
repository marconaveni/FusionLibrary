#include "window.h"
#include "platform_desktop_glfw.h"
#include "renderer.h"
#include "sprite.h"
#include "text.h"
#include <cmath>

namespace Fusion
{

    Window::Window()
    {
    }

    Window::~Window()
    {
    }

    void Window::InitWindow(const char *title, int width, int height)
    {
        m_Render = std::make_unique<Renderer>();
        m_Platform = std::make_unique<PlatformDesktopGLFW>();
        m_Platform->Init(title, width, height);
        m_Render->Init();
    }

    void Window::CloseWindow()
    {
    }

    bool Window::WindowShouldClose()
    {
        return m_Platform->WindowShouldClose();
    }

    bool Window::IsWindowResize()
    {
        return false;
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
        m_Render->BeginRender();
    }

    void Window::Draw(const Sprite &sprite)
    {
        m_Render->DrawTexture(sprite);
    }

    void Window::Draw(Text &text)
    {
        m_Render->DrawText(text);
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
}
