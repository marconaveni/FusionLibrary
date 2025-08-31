#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <memory>

#include "font.h"
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
    class Shader;
    class Font;

    class Window
    {
    public:

        Window();
        ~Window();

        bool InitWindow(const char* title, int width, int height);
        void Close();
        bool WindowShouldClose();
        bool IsWindowResize();
        Sizei GetWindowSize() const;
        void Clear(Color color);
        void BeginDrawing();
        void Draw(const Sprite& sprite, Shader* customShader = nullptr);
        void Draw(Text& text, Shader* customShader = nullptr);
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
        void BeginTextureMode(const RenderTexture& target);
        void EndTextureMode();
        void BeginMode2D(const Camera2D& camera);
        void EndMode2D();
        void BeginBlendMode(BlendMode mode);
        void EndBlendMode();

        // Timer
        double GetFrameTime() const;
        int GetFPS();
        double GetTime() const;
        void SetTargetFPS(int fps);

        Font& GetDefaultFont();

        void SetMainLoop(std::function<void()> loop);

    private:

        std::unique_ptr<Renderer> m_renderer;
        std::unique_ptr<Platform> m_platform;
        glm::mat4 m_defaultProjection; // Salvar a projeção da janela
        Font m_defaultFont;

        void WaitTime(double seconds);


        struct Time
        {
            double current = 0.0;
            double previous = 0.0;
            double frame = 0.0;
            double target = 0.0;
            double draw = 0;
            double update = 0;
            unsigned int frameCounter = 0;
            int fps = 0;
        };

        Time m_time;
    };

} // namespace Fusion

#endif