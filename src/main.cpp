// Em main.cpp
#include "window.h"
#include "sprite.h"
#include "font.h"
#include "text.h"
#include "core.h"
#include "input.h"

#include <format>
#include <print>

#if defined(FUSION_PLATFORM_WEB)
#include "platform.h" 
#include <emscripten/emscripten.h>
#endif

Fusion::Text text;
Fusion::Texture texture;
Fusion::Sprite player;
Fusion::Sprite player2;
float x = 5;
float y = 5;

// Função que representa um único frame do nosso jogo
void UpdateAndDrawFrame(Fusion::Window &window)
{

    Fusion::Vector2f pos = player.GetPosition();
    if (pos.x > 700 || pos.x < 0)
    {
        x = x * -1;
    }
    if (pos.y > 500 || pos.y < 0)
    {
        y = y * -1;
    }
    
    player.SetPosition(pos.x + x, pos.y + y);

    window.BeginDrawing();
    // Por enquanto, vamos limpar a tela com uma cor
    window.Clear({0.5f, 0.1f, 0.1f, 1.0f});
    window.DrawCircle({100,100}, 50, {1.0f, 0.1f, 0.1f, 1.0f});


    window.Draw(player);
    text.SetText("teste"); 
    window.Draw(text);

    // Futuramente, as chamadas de desenho virão aqui
    window.EndDrawing();
}


int main()
{

    Fusion::Window window;
    window.InitWindow("Teste de Input", 800, 600);
    
    window.GetDefaultFont().SetSmooth(false);
    text.SetFont(window.GetDefaultFont());
    text.SetPosition(10, 10);
    text.SetScale(4);
    
    texture.LoadFromFile("../assets/test2.png");
    player.SetTexture (texture);
    player2.SetTexture(texture);
    player.SetPosition({350, 250});
    player2.SetPosition({350, 250});
    player.SetSize({100, 100});
    player2.SetSize({100, 100});


#if defined(FUSION_PLATFORM_WEB)

    window.SetMainLoop([&]() {
        UpdateAndDrawFrame(window);
    });

#else

    window.SetTargetFPS(60);



    float speed = 5.0f; // pixels por segundo

    while (!window.WindowShouldClose())
    {

        Fusion::Core::PollEvents();
        // --- ATUALIZAÇÃO ---
        float frameTime = window.GetFrameTime();
        Fusion::Vector2f pos = player.GetPosition();

        if (window.GetInput()->IsKeyDown(GLFW_KEY_W) || window.GetInput()->IsGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_DPAD_UP))
        {
            pos.y -= speed;
            window.GetDefaultFont().SetSmooth(true);
        }
        if (window.GetInput()->IsKeyDown(GLFW_KEY_S) || window.GetInput()->IsGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_DPAD_DOWN))
        {
            pos.y += speed;
            window.GetDefaultFont().SetSmooth(false);
        }
        if (window.GetInput()->IsKeyDown(GLFW_KEY_A) || window.GetInput()->IsGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_DPAD_LEFT))
            pos.x -= speed;
        if (window.GetInput()->IsKeyDown(GLFW_KEY_D) || window.GetInput()->IsGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT))
            pos.x += speed;

        player.SetPosition(pos);

        // --- DESENHO ---
        window.BeginDrawing();
        window.Clear({0.1f, 0.1f, 0.1f, 1.0f});

        window.Draw(player);
        
        std::string fps = std::format("{} fps",  window.GetFPS());
        text.SetText(fps);
        std::println("{}",fps);
        window.Draw(text);

        window.EndDrawing();
    }

    texture.Unload();
    window.Close();
#endif
    return 0;
}
