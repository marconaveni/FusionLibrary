// Em main.cpp
#include "window.h"
#include "sprite.h"
#include "font.h"
#include "text.h"
#include "core.h"

#include <iostream>

#if defined(FUSION_PLATFORM_WEB)
#include "platform.h" 
#include <emscripten/emscripten.h>
#endif

// Função que representa um único frame do nosso jogo
void UpdateAndDrawFrame(Fusion::Window &window)
{
    window.BeginDrawing();
    // Por enquanto, vamos limpar a tela com uma cor
    window.Clear({0.5f, 0.1f, 0.1f, 1.0f});
    window.DrawCircle({100,100}, 50, {1.0f, 0.1f, 0.1f, 1.0f});
    // Futuramente, as chamadas de desenho virão aqui
    window.EndDrawing();
}

int main()
{

    Fusion::Window window;
    window.InitWindow("Teste de Input", 800, 600);

#if defined(FUSION_PLATFORM_WEB)

    window.SetMainLoop([&]() {
        UpdateAndDrawFrame(window);
    });

#else

    window.SetTargetFPS(60);

    // Fusion::Font font("../assets/NataSans-Regular.ttf", 32, 224);
    Fusion::Text text(window.GetDefaultFont());
    text.SetPosition(10, 10);
    text.SetScale(4);
    Fusion::Texture texture("../assets/test2.png");
    Fusion::Sprite player(texture);
    Fusion::Sprite player2(texture);
    player.SetPosition({350, 250});
    player2.SetPosition({350, 250});
    player.SetSize({100, 100});
    player2.SetSize({100, 100});

    float speed = 5.0f; // pixels por segundo

    while (!window.WindowShouldClose())
    {

        Fusion::Core::PollEvents();
        // --- ATUALIZAÇÃO ---
        float frameTime = window.GetFrameTime();
        Fusion::Vector2f pos = player.GetPosition();

        if (window.IsKeyDown(GLFW_KEY_W) || window.IsGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_DPAD_UP))
        {
            pos.y -= speed;
            window.GetDefaultFont().SetSmooth(true);
        }
        if (window.IsKeyDown(GLFW_KEY_S) || window.IsGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_DPAD_DOWN))
        {
            pos.y += speed;
            window.GetDefaultFont().SetSmooth(false);
        }
        if (window.IsKeyDown(GLFW_KEY_A) || window.IsGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_DPAD_LEFT))
            pos.x -= speed;
        if (window.IsKeyDown(GLFW_KEY_D) || window.IsGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT))
            pos.x += speed;

        player.SetPosition(pos);

        // --- DESENHO ---
        window.BeginDrawing();
        window.Clear({0.1f, 0.1f, 0.1f, 1.0f});

        window.Draw(player);
        std::string fps = std::to_string(window.GetFPS());
        text.SetText(fps);
        std::cout << window.GetFPS() << "\n";
        window.Draw(text);

        window.EndDrawing();
    }

    texture.Unload();
    window.Close();
#endif
    return 0;
}
