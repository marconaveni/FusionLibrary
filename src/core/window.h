#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include "fusion_math.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Fusion
{

    class Renderer;
    class Platform;
    class Sprite;
    class Text;
    class RenderTexture;
    class Camera2D;

    class Window
    {
    public:
        Window();
        ~Window();

        void InitWindow(const char *title, int width, int height);
        bool WindowShouldClose();
        bool IsWindowResize();
        Sizei GetWindowSize() const;
        void Clear(Color color);
        void BeginDrawing();
        void Draw(const Sprite &sprite);
        void Draw(Text &text);
        void EndDrawing();
        void BeginScissorMode(int x, int y, int width, int height);
        void EndScissorMode();
        void BeginTextureMode(const RenderTexture &target);
        void EndTextureMode();
        void BeginMode2D(const Camera2D &camera);
        void EndMode2D();
        double GetTime() const;

    private:
        std::unique_ptr<Renderer> m_Render;
        std::unique_ptr<Platform> m_Platform;
        glm::mat4 m_DefaultProjection; // Salvar a projeção da janela
    };

}

#endif