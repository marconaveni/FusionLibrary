#ifndef PLATFORM_DESKTOP_GLFW_H
#define PLATFORM_DESKTOP_GLFW_H

#include "core.h"
#include "platform.h"

namespace Fusion
{

    class PlatformDesktopGLFW : public Platform
    {
    public:

        virtual void Init(const char* title, int width, int height) override;
        virtual bool IsWindowActive() override;
        virtual void MakeContextCurrent() override;
        virtual bool WindowShouldClose() override;
        virtual void PollEventsAndUpdate() override;
        virtual void InputEvents() override;
        virtual void Shutdown() override;
        virtual void Clear(Color color) override;
        virtual Sizei GetWindowSize() const override;
        virtual bool IsWindowResized() override;
        virtual double GetTime() const override;

    private:

        GLFWwindow* m_window = nullptr;
        int m_viewPortWidth = 0;
        int m_viewPortHeight = 0;
        bool m_isResized = false;

        // GLFW callback
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void JoystickCallback(int jid, int event);

        void OnFramebufferResize(int width, int height);
    };

} // namespace Fusion

#endif