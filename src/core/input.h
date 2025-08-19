#ifndef INPUT_H
#define INPUT_H

#include "fusion_math.h"
#include "gamepad.h"
#include "keyboard.h"
#include "mouse.h"
#include <string>

namespace Fusion
{



    class Input
    {

    public:
        Input();

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
        Vector2f GetMousePosition() const;
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
        void UpdateMousePosition(Vector2f position);
        void ResetMouseWhellMove();

    private:
        Gamepad m_gamePads[gamePadCount];
        Keyboard m_keyboard;
        Mouse m_mouse;
    };
} // namespace Fusion

#endif // INPUT_H