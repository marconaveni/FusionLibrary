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
        m_Render->DrawTexture(*sprite.m_Texture, sprite.m_Source, sprite.m_Position,
             sprite.m_Origin, sprite.m_Rotation, sprite.m_Color);
    }

    void Window::Draw(Text &text)
    {  
        m_Render->DrawText(
            *text.m_Font, 
            text.m_Text, 
            Vector2f {text.m_Position.x, text.m_Position.y}, 
            text.m_Origin, 
            text.m_Rotation, 
            std::max(text.GetSize().width, text.GetSize().height),  
            text.m_Spacing, 
            text.m_Color
        );
    }

    void Window::EndDrawing()
    {
        m_Render->EndRender();
        m_Platform->SwapBuffersPollEvents();
    }

    Vector2f Window::MeasureText(const Text &text, float spacing) const
    {
        return m_Render->MeasureText(*text.m_Font, text.GetText(), std::max(text.GetSize().width, text.GetSize().height), spacing);
    }
}
