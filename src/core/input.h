#ifndef INPUT_H
#define INPUT_H

#include <string>

#include "fusion_math.h"
#include "gamepad.h"
#include "keyboard.h"
#include "mouse.h"

namespace Fusion
{

    class Input
    {

    public:

        Input(const Input&) = delete;
        Input& operator=(const Input&) = delete; // Deleted copy assignment
        static Input& GetInstance();             // Singleton inputs não precisam ter varias instancias


        bool IsGamepadAvailable(int gamepad) const;
        std::string GetGamepadName(int gamepad) const;
        bool IsGamepadButtonPressed(int gamepad, int button) const;
        bool IsGamepadButtonDown(int gamepad, int button) const;
        bool IsGamepadButtonReleased(int gamepad, int button) const;
        float GetGamepadAxisMovement(int gamepad, int axis) const;

        bool IsKeyPressed(int key) const;
        bool IsKeyDown(int key) const;
        bool IsKeyReleased(int key) const;

        bool IsMouseButtonPressed(int button) const;
        bool IsMouseButtonDown(int button) const;
        bool IsMouseButtonReleased(int button) const;
        Vector2i GetMousePosition() const;
        float GetMouseWheelMove() const;

        void UpdateGamepadPreviousState(int gamepad);
        void UpdateGamePadCurrentState(int gamepad, int button, bool state);
        void UpdateGamePadCurrentStateAxis(int gamepad, int button, float state);
        void RegisterGamePad(int gamepad, std::string name);
        void UnRegisterGamePad(int gamepad);

        void UpdateKeyboardPreviousState();
        void UpdateKeyboardCurrentState(int key, bool state);

        void UpdateMousePreviousState();
        void UpdateMouseCurrentState(int button, bool state);
        void UpdateMouseWhellMove(float move);
        void UpdateMousePosition(Vector2i position);
        void ResetMouseWhellMove();

    private:

        Input();
        ~Input();
        Gamepad::GamepadData m_gamePads[Gamepad::gamePadCount];
        Keyboard::KeyboardData m_keyboard;
        Mouse::MouseData m_mouse;
    };
} // namespace Fusion

#endif // INPUT_H