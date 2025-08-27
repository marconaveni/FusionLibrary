#include "mouse.h"

#include "input.h"

namespace Fusion::Mouse
{

    bool IsMouseButtonPressed(int button)
    {
        return Input::GetInstance().IsMouseButtonPressed(button);
    }

    bool IsMouseButtonDown(int button)
    {
        return Input::GetInstance().IsMouseButtonDown(button);
    }

    bool IsMouseButtonReleased(int button)
    {
        return Input::GetInstance().IsMouseButtonReleased(button);
    }

    Vector2i GetMousePosition()
    {
        return Input::GetInstance().GetMousePosition();
    }

    float GetMouseWheelMove()
    {
        return Input::GetInstance().GetMouseWheelMove();
    }

} // namespace Fusion::Mouse