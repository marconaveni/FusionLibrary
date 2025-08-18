#ifndef MOUSE_H
#define MOUSE_H

#include <array>
#include "fusion_math.h"

namespace Fusion
{

    static constexpr unsigned int mouseButtonsCount{8}; // Maximum number of supported keys

    struct Mouse
    {
        std::array<bool, mouseButtonsCount> currentState{};
        std::array<bool, mouseButtonsCount> previousState{};
        Vector2f position{};
        float wheelMove{};
        float previousWheelMove{};
    };

} // namespace Fusion

#endif
