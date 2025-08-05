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

    Vector2f Window::GetWindowSize()
    {
        return Vector2f();
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

}
