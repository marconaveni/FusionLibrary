#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <array>

namespace Fusion
{

    static constexpr unsigned int keyCount{512}; // Maximum number of supported keys

    struct Keyboard
    {
        std::array<bool, keyCount> currentState{};
        std::array<bool, keyCount> previousState{};
    };

} // namespace Fusion

#endif
