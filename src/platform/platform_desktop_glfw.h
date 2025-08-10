#ifndef PLATFORM_DESKTOP_GLFW_H
#define PLATFORM_DESKTOP_GLFW_H

#define GLFW_INCLUDE_NONE // Disable the standard OpenGL header inclusion on GLFW3

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "platform.h"

#define MAX_KEYS 512
#define MAX_MOUSE_BUTTONS 8

#define MAX_GAMEPADS 4
#define MAX_GAMEPAD_BUTTONS 15
#define MAX_GAMEPAD_AXES 6

namespace Fusion
{

    class PlatformDesktopGLFW : public Platform
    {
    public:
        virtual void Init(const char *title, int width, int height) override;
        virtual bool IsWindowActive() override;
        virtual bool WindowShouldClose() override;
        virtual void PollEventsAndUpdate() override;
        virtual void InputEvents() override;
        virtual void Shutdown() override;
        virtual void Clear(Color color) override;
        virtual Sizei GetWindowSize() const override;
        virtual bool IsWindowResized() override;
        virtual void UpdateTime() override;
        virtual float GetFrameTime() const override;
        virtual int GetFPS() const override;
        virtual void SetTargetFPS(int fps) override;

        virtual bool IsKeyPressed(int key) const override;
        virtual bool IsKeyDown(int key) const override;
        virtual bool IsKeyReleased(int key) const override;

        virtual bool IsMouseButtonPressed(int button) const override;
        virtual bool IsMouseButtonDown(int button) const override;
        virtual bool IsMouseButtonReleased(int button) const override;
        virtual Vector2f GetMousePosition() const override;
        virtual float GetMouseWheelMove() const override;

        virtual bool IsGamepadAvailable(int gamepad) const override;
        virtual const char *GetGamepadName(int gamepad) const override;
        virtual bool IsGamepadButtonPressed(int gamepad, int button) const override;
        virtual bool IsGamepadButtonDown(int gamepad, int button) const override;
        virtual bool IsGamepadButtonReleased(int gamepad, int button) const override;
        virtual float GetGamepadAxisMovement(int gamepad, int axis) const override;

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

        bool m_keyCurrentState[MAX_KEYS];
        bool m_keyPreviousState[MAX_KEYS];
        bool m_mouseCurrentState[MAX_MOUSE_BUTTONS];
        bool m_mousePreviousState[MAX_MOUSE_BUTTONS];
        Vector2f m_mousePosition = {0.0f, 0.0f};
        float m_mouseWheelMove = 0.0f;
        float m_previousMouseWheelMove = 0.0f;

        // --- ARRAYS PARA GUARDAR O ESTADO DO GAMEPAD ---
        bool m_gamepadReady[MAX_GAMEPADS];
        float m_gamepadAxisState[MAX_GAMEPADS][MAX_GAMEPAD_AXES];
        bool m_gamepadButtonCurrentState[MAX_GAMEPADS][MAX_GAMEPAD_BUTTONS];
        bool m_gamepadButtonPreviousState[MAX_GAMEPADS][MAX_GAMEPAD_BUTTONS];

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