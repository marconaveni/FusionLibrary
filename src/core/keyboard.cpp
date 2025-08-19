#include "keyboard.h"
#include "input.h"

namespace Fusion::Keyboard
{

    bool IsKeyPressed(int key)
    {
        return Input::GetInstance().IsKeyPressed(key);
    }

    bool IsKeyDown(int key)
    {
        return Input::GetInstance().IsKeyDown(key);
    }

    bool IsKeyReleased(int key)
    {
        return Input::GetInstance().IsKeyReleased(key);
    }

} // namespace Fusion::Keyboard