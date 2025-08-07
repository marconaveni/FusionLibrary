#ifndef PLATFORM_DESKTOP_GLFW_H
#define PLATFORM_DESKTOP_GLFW_H

#define GLFW_INCLUDE_NONE // Disable the standard OpenGL header inclusion on GLFW3

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "platform.h"

namespace Fusion
{

    class PlatformDesktopGLFW : public Platform
    {
    public:
        virtual void Init(const char *title, int width, int height) override;
        virtual bool WindowShouldClose() override;
        virtual void SwapBuffersPollEvents() override;
        virtual void Shutdown() override;
        virtual void Clear(Color color) override;
        virtual Sizei GetWindowSize() const override;
        virtual bool IsWindowResized() override;
        virtual void UpdateTime() override;
        virtual float GetFrameTime() const override;
        virtual int GetFPS() const override;

    private:
        GLFWwindow *m_Window = nullptr;
        int m_ViewPortWidth = 0;
        int m_ViewPortHeight = 0;
        bool m_isResized = false;
        double m_CurrentTime = 0.0;
        double m_LastTime = 0.0;
        float m_FrameTime = 0.0f;
        int m_Fps = 0;

        static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
        void OnFramebufferResize(int width, int height);
    };

    // //void ProcessInput();

    // void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    // int InitPlatform(const char* title, int width, int height);

    // bool WindowShouldClose();

    // void ClosePlatform();

    // void EndDrawing();

    // void Clear(Color color);
}

#endif