#ifndef MOUSE_H
#define MOUSE_H

#include <array>

#include "fusion_math.h"

namespace Fusion::Mouse
{

    static constexpr unsigned int mouseButtonsCount{8}; // Maximum number of supported keys

    struct MouseData
    {
        std::array<bool, mouseButtonsCount> currentState{};
        std::array<bool, mouseButtonsCount> previousState{};
        Vector2i position{};
        float wheelMove{};
        float previousWheelMove{};
    };

    bool IsMouseButtonPressed(int button);
    bool IsMouseButtonDown(int button);
    bool IsMouseButtonReleased(int button);
    Vector2i GetMousePosition();
    float GetMouseWheelMove();

} // namespace Fusion::Mouse

#endif
