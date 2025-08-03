#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include "fusion_math.h"


namespace Fusion
{

    class Renderer;
    class Platform;
    class Sprite;
    class Text;

    class Window
    {
    public:
        Window();
        ~Window();

        void InitWindow(const char *title, int width, int height);
        bool WindowShouldClose();
        void CloseWindow();
        bool IsWindowResize();
        Vector2f GetWindowSize();
        void Clear(Color color);
        void BeginDrawing();
        void Draw(const Sprite& sprite);
        void Draw(Text& text);
        void EndDrawing();
        Vector2f MeasureText(const Text& text, float spacing) const;

    private:
        std::unique_ptr<Renderer> m_Render;
        std::unique_ptr<Platform> m_Platform;
    };

}

#endif