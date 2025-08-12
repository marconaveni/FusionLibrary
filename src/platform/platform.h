#ifndef PLATFORM_H
#define PLATFORM_H

#include "fusion_math.h" // Para Color, Vector2, etc.
#include <functional>

namespace Fusion
{
    class Platform
    {
    public:
        virtual ~Platform() = default;

        virtual void Init(const char *title, int width, int height) = 0;
        virtual bool IsWindowActive() = 0;
        virtual void MakeContextCurrent() = 0; 
        virtual bool WindowShouldClose() = 0;
        virtual void PollEventsAndUpdate() = 0;
        virtual void InputEvents() = 0;
        virtual void Shutdown() = 0;
        virtual void Clear(Color color) = 0;
        virtual Sizei GetWindowSize() const = 0;
        virtual bool IsWindowResized() = 0;
        virtual void UpdateTime() = 0;
        virtual float GetFrameTime() const = 0;
        virtual float GetTime() const = 0;
        virtual int GetFPS() const = 0;
        virtual void SetTargetFPS(int fps) = 0;

        // --- MÉTODOS DE INPUT ---
        // Teclado
        virtual bool IsKeyPressed(int key) const = 0;
        virtual bool IsKeyDown(int key) const = 0;
        virtual bool IsKeyReleased(int key) const = 0;

        // Mouse
        virtual bool IsMouseButtonPressed(int button) const = 0;
        virtual bool IsMouseButtonDown(int button) const = 0;
        virtual bool IsMouseButtonReleased(int button) const = 0;
        virtual Vector2f GetMousePosition() const = 0;
        virtual float GetMouseWheelMove() const = 0;

        // Gamepad
        virtual bool IsGamepadAvailable(int gamepad) const = 0;
        virtual const char *GetGamepadName(int gamepad) const = 0;
        virtual bool IsGamepadButtonPressed(int gamepad, int button) const = 0;
        virtual bool IsGamepadButtonDown(int gamepad, int button) const = 0;
        virtual bool IsGamepadButtonReleased(int gamepad, int button) const = 0;
        virtual float GetGamepadAxisMovement(int gamepad, int axis) const = 0;


        virtual void SetMainLoop(std::function<void()> loop) {};
    

    };
}

#endif