#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <array>

namespace Fusion::Keyboard
{

    static constexpr unsigned int keyCount{512}; // Maximum number of supported keys

    struct KeyboardData
    {
        std::array<bool, keyCount> currentState{};
        std::array<bool, keyCount> previousState{};
    };

    bool IsKeyPressed(int key);
    bool IsKeyDown(int key);
    bool IsKeyReleased(int key);

} // namespace Fusion::Keyboard

#endif
