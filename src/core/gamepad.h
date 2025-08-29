#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <array>
#include <string>
#include <string_view>

namespace Fusion::Gamepad
{

    // Gamepad buttons
    enum Button
    {
        UNKNOWN = 0,           // Unknown button, just for error checking
        LEFT_FACE_UP = 1,      // Gamepad left DPAD up button
        LEFT_FACE_RIGHT = 2,   // Gamepad left DPAD right button
        LEFT_FACE_DOWN = 3,    // Gamepad left DPAD down button
        LEFT_FACE_LEFT = 4,    // Gamepad left DPAD left button
        RIGHT_FACE_UP = 5,     // Gamepad right button up (PS3: Triangle, Xbox: Y)
        RIGHT_FACE_RIGHT = 6,  // Gamepad right button right (PS3: Circle, Xbox: B)
        RIGHT_FACE_DOWN = 7,   // Gamepad right button down (PS3: Cross, Xbox: A)
        RIGHT_FACE_LEFT = 8,   // Gamepad right button left (PS3: Square, Xbox: X)
        LEFT_SHOULDER = 9,     // Gamepad top/back (PS3: L1, Xbox: LB)
        LEFT_TRIGGER = 10,     // Gamepad top/back (PS3: L2, Xbox: LT)
        RIGHT_SHOULDER = 11,   // Gamepad top/back (PS3: R1, Xbox: RB)
        RIGHT_TRIGGER = 12,    // Gamepad top/back (PS3: R2, Xbox: RT)
        MIDDLE_LEFT = 13,      // Gamepad center buttons, left one (PS3: Select, Xbox: Back)
        MIDDLE = 14,           // Gamepad center buttons, middle one (PS3: PS, Xbox: XBOX(Guide))
        MIDDLE_RIGHT = 15,     // Gamepad center buttons, right one (PS3: Start, Xbox: Back)
        LEFT_THUMB_STICK = 16, // Gamepad joystick analog stick pressed button left
        RIGHT_THUMB_STICK = 17 // Gamepad joystick analog stick pressed button right
    };

    // Gamepad axes
    enum Axis
    {
        AXIS_LEFT_X = 0,       // Gamepad left stick X axis
        AXIS_LEFT_Y = 1,       // Gamepad left stick Y axis
        AXIS_RIGHT_X = 2,      // Gamepad right stick X axis
        AXIS_RIGHT_Y = 3,      // Gamepad right stick Y axis
        AXIS_LEFT_TRIGGER = 4, // Gamepad back trigger left, pressure level: [-1 to 1]
        AXIS_RIGHT_TRIGGER = 5 // Gamepad back trigger right, pressure level: [-1 to 1]
    };

    static constexpr unsigned int gamePadCount{8};        // Maximum number of supported joysticks
    static constexpr unsigned int gamePadButtonCount{32}; // Maximum number of supported buttons
    static constexpr unsigned int gamePadAxisCount{8};    // Maximum number of supported axes
    static constexpr std::string_view gamePadDefaultName{"No Joystick"};

    struct GamepadData
    {
        bool ready;
        std::array<float, gamePadAxisCount> axisState{};
        std::array<bool, gamePadButtonCount> currentState{};
        std::array<bool, gamePadButtonCount> previousState{};
        std::string name{gamePadDefaultName}; // default name
    };

    bool IsGamepadAvailable(int gamepad);
    std::string GetGamepadName(int gamepad);
    bool IsGamepadButtonPressed(int gamepad, int button);
    bool IsGamepadButtonDown(int gamepad, int button);
    bool IsGamepadButtonReleased(int gamepad, int button);
    float GetGamepadAxisMovement(int gamepad, int axis);

} // namespace Fusion::Gamepad

#endif
