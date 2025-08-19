#include "gamepad.h"
#include "input.h"

namespace Fusion::Gamepad
{
    bool IsGamepadAvailable(int gamepad)
    {
        return Input::GetInstance().IsGamepadAvailable(gamepad);
    }

    std::string GetGamepadName(int gamepad)
    {
        return Input::GetInstance().GetGamepadName(gamepad);
    }

    bool IsGamepadButtonPressed(int gamepad, int button)
    {
        return Input::GetInstance().IsGamepadButtonPressed(gamepad, button);
    }

    bool IsGamepadButtonDown(int gamepad, int button)
    {
        return Input::GetInstance().IsGamepadButtonDown(gamepad, button);
    }

    bool IsGamepadButtonReleased(int gamepad, int button)
    {
        return Input::GetInstance().IsGamepadButtonReleased(gamepad, button);
    }

    float GetGamepadAxisMovement(int gamepad, int axis)
    {
        return Input::GetInstance().GetGamepadAxisMovement(gamepad, axis);
    }

} // namespace Fusion::Gamepad
