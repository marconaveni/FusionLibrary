#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <functional>
#include "fusion_math.h"
#include "glm/gtc/matrix_transform.hpp"
#include "font.h"

namespace Fusion
{

    class Renderer;
    class Platform;
    class Sprite;
    class Text;
    class RenderTexture;
    class Camera2D;
    class Shader;
    class Font;

    class Window
    {
    public:
        Window();
        ~Window();

        void InitWindow(const char *title, int width, int height);
        void Close();
        bool WindowShouldClose();
        bool IsWindowResize();
        Sizei GetWindowSize() const;
        void Clear(Color color);
        void BeginDrawing();
        void Draw(const Sprite &sprite, Shader *customShader = nullptr);
        void Draw(Text &text, Shader *customShader = nullptr);
        void DrawRectangle(int x, int y, int width, int height, Color color);
        void DrawCircle(int centerX, int centerY, float radius, Color color);
        void DrawCircle(Vector2f center, float radius, Color color);
        void DrawCircleLines(int centerX, int centerY, float radius, Color color);
        void DrawCircleLines(Vector2f center, float radius, Color color);
        void DrawTriangle(Vector2f v1, Vector2f v2, Vector2f v3, Color color);
        void DrawLine(Vector2f startPos, Vector2f endPos, float thick, Color color);
        void DrawRectangleLines(int x, int y, int width, int height, float thick, Color color);
        void EndDrawing();
        void BeginScissorMode(int x, int y, int width, int height);
        void EndScissorMode();
        void BeginTextureMode(const RenderTexture &target);
        void EndTextureMode();
        void BeginMode2D(const Camera2D &camera);
        void EndMode2D();
        void BeginBlendMode(BlendMode mode);
        void EndBlendMode();

        // Timer
        float GetFrameTime() const;
        int GetFPS() const;
        double GetTime() const;
        void SetTargetFPS(int fps);

        Font &GetDefaultFont();

        void SetMainLoop(std::function<void()> loop);

    private:
        std::unique_ptr<Renderer> m_Render;
        std::unique_ptr<Platform> m_Platform;
        glm::mat4 m_DefaultProjection; // Salvar a projeção da janela
        Font m_defaultFont;
    };

}

#endif