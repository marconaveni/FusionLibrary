#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <string>
#include <string_view>
#include <array>

namespace Fusion
{
    
    static constexpr unsigned int gamePadCount{8};        // Maximum number of supported joysticks
    static constexpr unsigned int gamePadButtonCount{32}; // Maximum number of supported buttons
    static constexpr unsigned int gamePadAxisCount{8};    // Maximum number of supported axes
    static constexpr std::string_view gamePadDefaultName{"No Joystick"};

    struct Gamepad
    {
        bool ready;
        std::array<float, gamePadAxisCount> axisState{};
        std::array<bool, gamePadButtonCount> currentState{};
        std::array<bool, gamePadButtonCount> previousState{};
        std::string name{gamePadDefaultName}; // default name
    };

} // namespace Fusion

#endif
