#ifndef FUSION_PLATFORM_WEB_H
#define FUSION_PLATFORM_WEB_H

#include "platform.h" // Inclui a interface base
#include "input.h" 
#include <functional>
#include <emscripten/html5.h>

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
        virtual void InputEvents() override;
        virtual void Shutdown() override {}
        virtual void Clear(Color color) override;
        virtual Sizei GetWindowSize() const override { return Sizei(); }
        virtual bool IsWindowResized() override { return false; }
        virtual void UpdateTime() override {}
        virtual float GetFrameTime() const override { return 0.0f; }
        virtual float GetTime() const override { return 0.0f; }
        virtual int GetFPS() const override { return 0; }
        virtual void SetTargetFPS(int fps) override {}
        virtual Input* GetInput() override { return nullptr; }

        static EM_BOOL EmscriptenGamepadCallback(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData);

    private:
        std::function<void()> m_MainLoop;
    };
}

#endif // FUSION_PLATFORM_WEB_H