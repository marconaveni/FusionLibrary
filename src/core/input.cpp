#include "input.h"

#include <cmath>   // para fabs

namespace Fusion
{

    Input::Input()
    {
    }

    Input::~Input()
    {
    }

    Input& Input::GetInstance()
    {
        static Input instance;
        return instance;
    }

    bool Input::IsGamepadAvailable(int gamepad) const
    {
        return (gamepad < Gamepad::gamePadCount) && m_gamePads[gamepad].ready;
    }

    std::string Input::GetGamepadName(int gamepad) const
    {
        return m_gamePads[gamepad].name; //     glfwGetGamepadName(gamepad);
    }

    bool Input::IsGamepadButtonPressed(int gamepad, int button) const
    {
        return IsGamepadAvailable(gamepad) && m_gamePads[gamepad].currentState[button] &&
               !m_gamePads[gamepad].previousState[button];
    }

    bool Input::IsGamepadButtonDown(int gamepad, int button) const
    {
        return IsGamepadAvailable(gamepad) && m_gamePads[gamepad].currentState[button];
    }

    bool Input::IsGamepadButtonReleased(int gamepad, int button) const
    {
        return IsGamepadAvailable(gamepad) && !m_gamePads[gamepad].currentState[button] &&
               m_gamePads[gamepad].previousState[button];
    }

    float Input::GetGamepadAxisMovement(int gamepad, int axis) const
    {
        float value = 0.0f;
        if (IsGamepadAvailable(gamepad) && axis < Gamepad::gamePadAxisCount)
        {
            value = m_gamePads[gamepad].axisState[axis];
            if (fabs(value) < 0.1f)
            {
                value = 0.0f; // Aplica um "deadzone" para evitar movimentos fantasmas
            }
        }
        return value;
    }

    void Input::UpdateGamepadPreviousState(int gamepad)
    {
        // Copia o estado atual para o anterior
        for (int j = 0; j < Gamepad::gamePadButtonCount; j++)
        {
            m_gamePads[gamepad].previousState[j] = m_gamePads[gamepad].currentState[j];
        }
    }

    void Input::UpdateGamePadCurrentState(int gamepad, int button, bool state)
    {
        // Atualiza o estado dos botões
        m_gamePads[gamepad].currentState[button] = state;
    }

    void Input::UpdateGamePadCurrentStateAxis(int gamepad, int button, float state)
    {
        // Atualiza o estado dos eixos
        m_gamePads[gamepad].axisState[button] = state;
    }

    void Input::RegisterGamePad(int gamepad, std::string name)
    {
        m_gamePads[gamepad].name = name;
        m_gamePads[gamepad].ready = true;
    }

    void Input::UnRegisterGamePad(int gamepad)
    {
        m_gamePads[gamepad].name = Gamepad::gamePadDefaultName;
        m_gamePads[gamepad].ready = false;

        m_gamePads[gamepad].axisState.fill(0.0f);
        m_gamePads[gamepad].currentState.fill(false);
        m_gamePads[gamepad].previousState.fill(false);
    }

    void Input::UpdateKeyboardPreviousState()
    {
        // Atualiza o estado anterior das teclas e botões
        for (int i = 0; i < Keyboard::keyCount; i++)
        {
            m_keyboard.previousState[i] = m_keyboard.currentState[i];
        }
    }

    void Input::UpdateKeyboardCurrentState(int key, bool state)
    {
        m_keyboard.currentState[key] = state;
    }

    void Input::UpdateMousePreviousState()
    {
        for (int i = 0; i < Mouse::mouseButtonsCount; i++)
        {
            m_mouse.previousState[i] = m_mouse.currentState[i];
        }
    }

    void Input::UpdateMouseCurrentState(int button, bool state)
    {
        m_mouse.currentState[button] = state;
    }

    void Input::UpdateMouseWhellMove(float move)
    {
        m_mouse.wheelMove = move;
    }

    void Input::UpdateMousePosition(Vector2i position)
    {
        m_mouse.position = position;
    }

    void Input::ResetMouseWhellMove()
    {
        m_mouse.wheelMove = 0.0f;
    }

    bool Input::IsKeyPressed(int key) const
    {
        return m_keyboard.currentState[key] && !m_keyboard.previousState[key];
    }

    bool Input::IsKeyDown(int key) const
    {
        return m_keyboard.currentState[key];
    }

    bool Input::IsKeyReleased(int key) const
    {
        return !m_keyboard.currentState[key] && m_keyboard.previousState[key];
    }

    bool Input::IsMouseButtonPressed(int button) const
    {
        return m_mouse.currentState[button] && !m_mouse.previousState[button];
    }

    bool Input::IsMouseButtonDown(int button) const
    {
        return m_mouse.currentState[button];
    }

    bool Input::IsMouseButtonReleased(int button) const
    {
        return !m_mouse.currentState[button] && m_mouse.previousState[button];
    }

    Vector2i Input::GetMousePosition() const
    {
        return m_mouse.position;
    }

    float Input::GetMouseWheelMove() const
    {
        return m_mouse.wheelMove;
    }

} // namespace Fusion
