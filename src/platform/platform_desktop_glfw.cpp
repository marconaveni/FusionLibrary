#include "platform_desktop_glfw.h"
#include <iostream>

struct WindowStatus
{
    int viewPortWidth;
    int viewPortHeight;
};

namespace Fusion
{

    void PlatformDesktopGLFW::Init(const char *title, int width, int height)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
        m_ViewPortWidth = width;
        m_ViewPortHeight = height;

        if (!m_Window)
        {
            std::cerr << "Erro ao criar janela\n";
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, this);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Erro ao inicializar GLAD\n";
            return;
        }

        glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
    }

    bool PlatformDesktopGLFW::WindowShouldClose()
    {
        return glfwWindowShouldClose(m_Window);
    }

    void PlatformDesktopGLFW::SwapBuffersPollEvents()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void PlatformDesktopGLFW::Shutdown()
    {
    }

    void PlatformDesktopGLFW::Clear(Color color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    Sizei PlatformDesktopGLFW::GetWindowSize() const
    {
        return Sizei(m_ViewPortWidth, m_ViewPortHeight);
    }

    void PlatformDesktopGLFW::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        PlatformDesktopGLFW *platform = static_cast<PlatformDesktopGLFW*>(glfwGetWindowUserPointer(window));

        if (platform)
        {
            platform->OnFramebufferResize(width, height);
        }
    }
    void PlatformDesktopGLFW::OnFramebufferResize(int width, int height)
    {
        glViewport(0, height - m_ViewPortHeight, m_ViewPortWidth, m_ViewPortHeight);
    }
}

// GLFWwindow *window;
// WindowStatus windowStatus;

// void Fusion::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
// {
//     glViewport(0, height - windowStatus.viewPortHeight, windowStatus.viewPortWidth, windowStatus.viewPortHeight);
// }

// int Fusion::InitPlatform(const char *title, int width, int height)
// {
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//     window = glfwCreateWindow(width, height, title, NULL, NULL);
//     windowStatus.viewPortWidth = width;
//     windowStatus.viewPortHeight = height;

//     if (!window)
//     {
//         std::cerr << "Erro ao criar janela\n";
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);

//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         std::cerr << "Erro ao inicializar GLAD\n";
//         return -1;
//     }

//     return 0;
// }

// bool Fusion::WindowShouldClose()
// {
//     return glfwWindowShouldClose(window);
// }

// void Fusion::ClosePlatform()
// {
// }

// void Fusion::EndDrawing()
// {
//     glfwSwapBuffers(window);
//     glfwPollEvents();
// }

// void Fusion::Clear(Color color)
// {
//     glClearColor(color.r, color.g, color.b, color.a);
//     glClear(GL_COLOR_BUFFER_BIT);
// }
