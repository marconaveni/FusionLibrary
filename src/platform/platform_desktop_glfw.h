#ifndef PLATFORM_DESKTOP_GLFW_H
#define PLATFORM_DESKTOP_GLFW_H

#include "core.h"
#include "platform.h"
#include "input.h"


namespace Fusion
{



    class PlatformDesktopGLFW : public Platform
    {
    public:
        virtual void Init(const char *title, int width, int height) override;
        virtual bool IsWindowActive() override;
        virtual void MakeContextCurrent() override;
        virtual bool WindowShouldClose() override;
        virtual void PollEventsAndUpdate() override;
        virtual void InputEvents() override;
        virtual void Shutdown() override;
        virtual void Clear(Color color) override;
        virtual Sizei GetWindowSize() const override;
        virtual bool IsWindowResized() override;
        virtual void UpdateTime() override;
        virtual float GetFrameTime() const override;
        virtual float GetTime() const override;
        virtual int GetFPS() const override;
        virtual void SetTargetFPS(int fps) override;



    private:
        GLFWwindow *m_Window = nullptr;
        int m_ViewPortWidth = 0;
        int m_ViewPortHeight = 0;
        bool m_isResized = false;
        double m_CurrentTime = 0.0;
        double m_PreviousTime = 0.0;
        double m_FrameTime = 0.0;
        double m_targetFrameTime = 0.0;
        int m_Fps = 0;

        //Input m_input;

        // GLFW callback
        static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
        static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
        static void MouseCursorPosCallback(GLFWwindow *window, double xpos, double ypos);
        static void MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
        static void JoystickCallback(int jid, int event);

        void OnFramebufferResize(int width, int height);
    };

}

#endif