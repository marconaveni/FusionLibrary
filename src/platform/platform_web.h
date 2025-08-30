#ifndef PLATFORM_WEB_H
#define PLATFORM_WEB_H

#include <emscripten/html5.h>
#include <functional>

#include "input.h"
#include "platform.h" // Inclui a interface base

struct GLFWwindow;

namespace Fusion
{
    // A nossa implementação da plataforma para a Web
    class PlatformWeb : public Platform
    {
    public:

        PlatformWeb();
        ~PlatformWeb() override;

        virtual void SetMainLoop(std::function<void()> loop) override;

        virtual void Init(const char* title, int width, int height) override;
        virtual bool IsWindowActive() override { return false; }
        virtual void MakeContextCurrent() override {};
        virtual bool WindowShouldClose() override { return false; }
        virtual void PollEventsAndUpdate() override;
        virtual void InputEvents() override;
        virtual void Shutdown() override {}
        virtual void Clear(Color color) override;
        virtual Sizei GetWindowSize() const override { return Sizei{m_ViewPortWidth, m_ViewPortHeight}; }
        virtual bool IsWindowResized() override { return false; }
        virtual float GetTime() const override;

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static EM_BOOL EmscriptenGamepadCallback(int eventType, const EmscriptenGamepadEvent* gamepadEvent,
                                                 void* userData);

    private:

        GLFWwindow* m_window = nullptr;
        std::function<void()> m_MainLoop;

        int m_ViewPortWidth = 0;
        int m_ViewPortHeight = 0;
    };
} // namespace Fusion

#endif // FUSION_PLATFORM_WEB_H