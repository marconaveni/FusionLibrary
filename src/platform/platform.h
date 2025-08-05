#ifndef PLATFORM_H
#define PLATFORM_H

#include "fusion_math.h" // Para Color, Vector2, etc.

namespace Fusion
{
    class Platform
    {
    public:
        virtual ~Platform() = default; 

        virtual void Init(const char* title, int width, int height) = 0;
        virtual bool WindowShouldClose() = 0;
        virtual void SwapBuffersPollEvents() = 0;
        virtual void Shutdown() = 0;
        virtual void Clear(Color color) = 0;
        virtual Sizei GetWindowSize() const = 0;
        virtual bool IsWindowResized() = 0;
    };
}

#endif