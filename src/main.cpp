// Em main.cpp
#include <format>
#include <iostream>

#include "core.h"
#include "font.h"
#include "gamepad.h"
#include "keyboard.h"
#include "render_texture.h"
#include "sprite.h"
#include "text.h"
#include "window.h"


#if defined(FUSION_PLATFORM_WEB)
#include <emscripten/emscripten.h>

#include "platform.h"
#endif

Fusion::Text text;
Fusion::Texture texture;
Fusion::Sprite player;
Fusion::Sprite player2;
Fusion::Sprite render;
float x = 5;
float y = 5;
float speed = 5.0f; // pixels por segundo


void GetPosition(Fusion::Vector2f& pos)
{

    pos.y += Fusion::Gamepad::GetGamepadAxisMovement(0, Fusion::Gamepad::Axis::AXIS_LEFT_Y) * speed;
    pos.x += Fusion::Gamepad::GetGamepadAxisMovement(0, Fusion::Gamepad::Axis::AXIS_LEFT_X) * speed;

    if (Fusion::Keyboard::IsKeyDown(Fusion::Keyboard::W) ||
        Fusion::Gamepad::IsGamepadButtonDown(0, Fusion::Gamepad::Button::LEFT_FACE_UP))
    {
        pos.y -= speed;
    }
    if (Fusion::Keyboard::IsKeyDown(Fusion::Keyboard::S) ||
        Fusion::Gamepad::IsGamepadButtonDown(0, Fusion::Gamepad::Button::LEFT_FACE_DOWN))
    {
        pos.y += speed;
    }
    if (Fusion::Keyboard::IsKeyDown(Fusion::Keyboard::A) ||
        Fusion::Gamepad::IsGamepadButtonDown(0, Fusion::Gamepad::Button::LEFT_FACE_LEFT))
    {
        pos.x -= speed;
    }

    if (Fusion::Keyboard::IsKeyDown(Fusion::Keyboard::D) ||
        Fusion::Gamepad::IsGamepadButtonDown(0, Fusion::Gamepad::Button::LEFT_FACE_RIGHT))
    {
        pos.x += speed;
    }
}


// Função que representa um único frame do nosso jogo
void UpdateAndDrawFrame(Fusion::Window& window)
{
    Fusion::Vector2f pos = player.GetPosition();
    GetPosition(pos);
    player.SetPosition(pos);


    window.BeginDrawing();
    // Por enquanto, vamos limpar a tela com uma cor
    window.Clear({0.5f, 0.8f, 0.1f, 1.0f});
    window.DrawCircle({100, 100}, 50, {1.0f, 0.1f, 0.1f, 1.0f});

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

    texture.LoadFromFile("../../assets/test2.png");
    player.SetTexture(texture);
    player2.SetTexture(texture);
    player.SetPosition({350, 250});
    player2.SetPosition({350, 250});
    player.SetSize({100, 100});
    player2.SetSize({100, 100});


#if defined(FUSION_PLATFORM_WEB)

    window.SetMainLoop([&]() { UpdateAndDrawFrame(window); });

#else

    window.SetTargetFPS(60);
    Fusion::RenderTexture target(100, 100);
    render.SetTexture(*target.GetTexture());
    render.SetSize({400, 400});

    while (!window.WindowShouldClose())
    {
        Fusion::Vector2f pos = player.GetPosition();
        GetPosition(pos);
        player.SetPosition(pos);


        window.BeginTextureMode(target);
        window.Clear({0.5f, 0.1f, 0.1f, 1.0f});

        window.Draw(player);

        window.EndTextureMode();


        // --- DESENHO ---
        window.BeginDrawing();
        window.Clear({0.1f, 0.1f, 0.1f, 1.0f});

        window.Draw(render);

        std::string fps = std::format("{} fps ", window.GetFPS());
        fps.shrink_to_fit();
        text.SetText(fps);

        window.Draw(text);

        window.EndDrawing();
    }

    //texture.Unload();
    //window.Close();
#endif
    return 0;
}
