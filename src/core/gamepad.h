#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <string>
#include <string_view>
#include <array>

namespace Fusion
{


// #define GLFW_GAMEPAD_BUTTON_A               0
// #define GLFW_GAMEPAD_BUTTON_B               1
// #define GLFW_GAMEPAD_BUTTON_X               2
// #define GLFW_GAMEPAD_BUTTON_Y               3
// #define GLFW_GAMEPAD_BUTTON_LEFT_BUMPER     4
// #define GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER    5
// #define GLFW_GAMEPAD_BUTTON_BACK            6
// #define GLFW_GAMEPAD_BUTTON_START           7
// #define GLFW_GAMEPAD_BUTTON_GUIDE           8
// #define GLFW_GAMEPAD_BUTTON_LEFT_THUMB      9
// #define GLFW_GAMEPAD_BUTTON_RIGHT_THUMB     10
// #define GLFW_GAMEPAD_BUTTON_DPAD_UP         11
// #define GLFW_GAMEPAD_BUTTON_DPAD_RIGHT      12
// #define GLFW_GAMEPAD_BUTTON_DPAD_DOWN       13
// #define GLFW_GAMEPAD_BUTTON_DPAD_LEFT       14
// #define GLFW_GAMEPAD_BUTTON_LAST            GLFW_GAMEPAD_BUTTON_DPAD_LEFT

// #define GLFW_GAMEPAD_BUTTON_CROSS       GLFW_GAMEPAD_BUTTON_A
// #define GLFW_GAMEPAD_BUTTON_CIRCLE      GLFW_GAMEPAD_BUTTON_B
// #define GLFW_GAMEPAD_BUTTON_SQUARE      GLFW_GAMEPAD_BUTTON_X
// #define GLFW_GAMEPAD_BUTTON_TRIANGLE    GLFW_GAMEPAD_BUTTON_Y



    enum GamepadButton
    {
        GAMEPAD_BUTTON_UNKNOWN = 0,      // Unknown button, just for error checking
        GAMEPAD_BUTTON_LEFT_FACE_UP,     // Gamepad left DPAD up button
        GAMEPAD_BUTTON_LEFT_FACE_RIGHT,  // Gamepad left DPAD right button
        GAMEPAD_BUTTON_LEFT_FACE_DOWN,   // Gamepad left DPAD down button
        GAMEPAD_BUTTON_LEFT_FACE_LEFT,   // Gamepad left DPAD left button
        GAMEPAD_BUTTON_RIGHT_FACE_UP,    // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT, // Gamepad right button right (i.e. PS3: Circle, Xbox: B)
        GAMEPAD_BUTTON_RIGHT_FACE_DOWN,  // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT,  // Gamepad right button left (i.e. PS3: Square, Xbox: X)
        GAMEPAD_BUTTON_LEFT_TRIGGER_1,   // Gamepad top/back trigger left (first), it could be a trailing button
        GAMEPAD_BUTTON_LEFT_TRIGGER_2,   // Gamepad top/back trigger left (second), it could be a trailing button
        GAMEPAD_BUTTON_RIGHT_TRIGGER_1,  // Gamepad top/back trigger right (first), it could be a trailing button
        GAMEPAD_BUTTON_RIGHT_TRIGGER_2,  // Gamepad top/back trigger right (second), it could be a trailing button
        GAMEPAD_BUTTON_MIDDLE_LEFT,      // Gamepad center buttons, left one (i.e. PS3: Select)
        GAMEPAD_BUTTON_MIDDLE,           // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
        GAMEPAD_BUTTON_MIDDLE_RIGHT,     // Gamepad center buttons, right one (i.e. PS3: Start)
        GAMEPAD_BUTTON_LEFT_THUMB,       // Gamepad joystick pressed button left
        GAMEPAD_BUTTON_RIGHT_THUMB       // Gamepad joystick pressed button right
    };

    
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
