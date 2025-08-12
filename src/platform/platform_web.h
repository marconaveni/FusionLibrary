#ifndef FUSION_PLATFORM_WEB_H
#define FUSION_PLATFORM_WEB_H

#include "platform.h" // Inclui a interface base
#include <functional>

namespace Fusion
{
    // A nossa implementação da plataforma para a Web
    class PlatformWeb : public Platform
    {
    public:
        PlatformWeb();
        ~PlatformWeb() override;

        virtual void SetMainLoop(std::function<void()> loop) override;

        virtual void Init(const char *title, int width, int height) override;
        virtual bool IsWindowActive() override { return false; }
        virtual void MakeContextCurrent() override {};
        virtual bool WindowShouldClose() override { return false; }
        virtual void PollEventsAndUpdate() override;
        virtual void InputEvents() override {}
        virtual void Shutdown() override {}
        virtual void Clear(Color color) override;
        virtual Sizei GetWindowSize() const override { return Sizei(); }
        virtual bool IsWindowResized() override { return false; }
        virtual void UpdateTime() override {}
        virtual float GetFrameTime() const override { return 0.0f; }
        virtual float GetTime() const override { return 0.0f; }
        virtual int GetFPS() const override { return 0; }
        virtual void SetTargetFPS(int fps) override {}

        // --- MÉTODOS DE INPUT ---
        // Teclado
        virtual bool IsKeyPressed(int key) const override { return false; }
        virtual bool IsKeyDown(int key) const override { return false; }
        virtual bool IsKeyReleased(int key) const override { return false; }

        // Mouse
        virtual bool IsMouseButtonPressed(int button) const override { return false; }
        virtual bool IsMouseButtonDown(int button) const override { return false; }
        virtual bool IsMouseButtonReleased(int button) const override { return false; }
        virtual Vector2f GetMousePosition() const override { return Vector2f(); }
        virtual float GetMouseWheelMove() const override { return 0.0f; }

        // Gamepad
        virtual bool IsGamepadAvailable(int gamepad) const override { return false; }
        virtual const char *GetGamepadName(int gamepad) const override { return ""; }
        virtual bool IsGamepadButtonPressed(int gamepad, int button) const override { return false; }
        virtual bool IsGamepadButtonDown(int gamepad, int button) const override { return false; }
        virtual bool IsGamepadButtonReleased(int gamepad, int button) const override { return false; }
        virtual float GetGamepadAxisMovement(int gamepad, int axis) const override { return 0.0f; }

    private:
        std::function<void()> m_MainLoop;
    };
}

#endif // FUSION_PLATFORM_WEB_H