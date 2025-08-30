#include "platform_desktop_glfw.h"

// #include <chrono> // Para std::chrono::duration
#include <cmath>
#include <iostream>
// #include <thread> // Para std::this_thread::sleep_for

#include "core.h"
#include "fusion_math.h"
#include "input.h"



namespace Fusion
{

    void PlatformDesktopGLFW::Init(const char* title, int width, int height)
    {
        // glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* shareWindow = nullptr;
        if (Core::s_sharedWindow != nullptr)
        {
            // Converte o Platform* genérico para a nossa implementação específica para obter o GLFWwindow*
            shareWindow = Core::s_sharedWindow;
        }

        m_Window = glfwCreateWindow(width, height, title, NULL, shareWindow);
        m_ViewPortWidth = width;
        m_ViewPortHeight = height;

        Core::s_sharedWindow = m_Window;

        if (!m_Window)
        {
            std::cerr << "Erro ao criar janela\n";
            glfwTerminate();
            return;
        }

        glfwSwapInterval(1); // enable v-sync

        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, this); // guarda um ponteiro da classe no glfw

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Erro ao inicializar GLAD\n";
            return;
        }

        // Registra as funções de callback no GLFW
        glfwSetKeyCallback(m_Window, KeyCallback);
        glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
        glfwSetCursorPosCallback(m_Window, MouseCursorPosCallback);
        glfwSetScrollCallback(m_Window, MouseScrollCallback);
        glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
        glfwSetJoystickCallback(JoystickCallback); // Registra o callback de conexão do joystick

        // Verifica quais gamepads já estão conectados no início
        for (int i = 0; i < Gamepad::gamePadCount; i++)
        {
            if (glfwJoystickPresent(i))
            {
                Input::GetInstance().RegisterGamePad(i, glfwGetJoystickName(i));
                std::cout << "INFO: Gamepad conectado na porta " << i << ": " << glfwGetJoystickName(i) << "\n";
            }
        }

        glfwSwapInterval(0);  // Desativa o VSync para usar nosso controle manual
    }

    bool PlatformDesktopGLFW::IsWindowActive()
    {
        return m_Window != nullptr;
    }

    void PlatformDesktopGLFW::MakeContextCurrent()
    {
        if(glfwGetCurrentContext() != m_Window) // só troca o contexto se for diferente 
        {
            glfwMakeContextCurrent(m_Window);  
        }       
    }

    bool PlatformDesktopGLFW::WindowShouldClose()
    {
        return glfwWindowShouldClose(m_Window);
    }

    void PlatformDesktopGLFW::PollEventsAndUpdate()
    {



        // Troca os buffers APÓS a espera
        glfwSwapBuffers(m_Window);

        // Atualiza o estado do input para o próximo quadro
        InputEvents();

        // Processa os eventos de janela (como fechar)
        glfwPollEvents();


    }

    void PlatformDesktopGLFW::InputEvents()
    {
        Input::GetInstance().UpdateKeyboardPreviousState();
        Input::GetInstance().UpdateMousePreviousState();
        Input::GetInstance().ResetMouseWhellMove();

        for (int i = 0; i < Gamepad::gamePadCount; i++)
        {

            // se não está presente, garanta o unregister
            if (!glfwJoystickPresent(i))
            {
                Input::GetInstance().UnRegisterGamePad(i);
                continue; // nada pra ler desse id
            }

            if (Input::GetInstance().IsGamepadAvailable(i) && glfwJoystickIsGamepad(i))
            {
                Input::GetInstance().UpdateGamepadPreviousState(i);
            }

            GLFWgamepadstate state;
            if (glfwGetGamepadState(i, &state))
            {
                // Atualiza o estado dos botões
                for (int j = 0; j < Gamepad::gamePadButtonCount; j++)
                {
                    int button = -1;

                    //10  e 12 GLFW não os considera como botões, mas sim como eixos
                    switch (j)
                    {
                        case GLFW_GAMEPAD_BUTTON_DPAD_UP: button = Gamepad::Button::LEFT_FACE_UP; break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT: button = Gamepad::Button::LEFT_FACE_RIGHT; break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_DOWN: button = Gamepad::Button::LEFT_FACE_DOWN; break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_LEFT: button = Gamepad::Button::LEFT_FACE_LEFT; break;
                        case GLFW_GAMEPAD_BUTTON_Y: button = Gamepad::Button::RIGHT_FACE_UP; break;
                        case GLFW_GAMEPAD_BUTTON_B: button = Gamepad::Button::RIGHT_FACE_RIGHT; break;
                        case GLFW_GAMEPAD_BUTTON_A: button = Gamepad::Button::RIGHT_FACE_DOWN; break;
                        case GLFW_GAMEPAD_BUTTON_X: button = Gamepad::Button::RIGHT_FACE_LEFT; break;
                        case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER: button = Gamepad::Button::LEFT_SHOULDER; break;
                        case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER: button = Gamepad::Button::RIGHT_SHOULDER; break;
                        case GLFW_GAMEPAD_BUTTON_BACK: button = Gamepad::Button::MIDDLE_LEFT; break;
                        case GLFW_GAMEPAD_BUTTON_GUIDE: button = Gamepad::Button::MIDDLE; break;
                        case GLFW_GAMEPAD_BUTTON_START: button = Gamepad::Button::MIDDLE_RIGHT; break;
                        case GLFW_GAMEPAD_BUTTON_LEFT_THUMB: button = Gamepad::Button::LEFT_THUMB_STICK; break;
                        case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB: button = Gamepad::Button::RIGHT_THUMB_STICK; break;
                        default: break;
                    }

                    if (button != -1) // Check for valid button
                    {
                        if ((state.buttons[j] == GLFW_PRESS))
                        {
                            //std::cout << "button values: " << button << " | j values: " << j << "\n";
                            Input::GetInstance().UpdateGamePadCurrentState(i, button, true);
                        }
                        else
                        {
                            //std::cout << j << "\n";
                            Input::GetInstance().UpdateGamePadCurrentState(i, button, false);
                        }
                    }
                }
                // Atualiza o estado dos eixos
                for (int j = 0; j < Gamepad::gamePadAxisCount; j++)
                {
                    Input::GetInstance().UpdateGamePadCurrentStateAxis(i, j, state.axes[j]);
                }

                // porque o GLFW não os considera como botões, mas sim como eixos

                constexpr int left = Gamepad::Axis::AXIS_RIGHT_TRIGGER;
                constexpr int right = Gamepad::Axis::AXIS_RIGHT_TRIGGER;

                const bool isLeftTrigger = (Input::GetInstance().GetGamepadAxisMovement(i, left) > 0.1);
                const bool isRightTrigger = (Input::GetInstance().GetGamepadAxisMovement(i, right) > 0.1);

                Input::GetInstance().UpdateGamePadCurrentState(i, Gamepad::Button::LEFT_TRIGGER, isLeftTrigger);
                Input::GetInstance().UpdateGamePadCurrentState(i, Gamepad::Button::RIGHT_TRIGGER, isRightTrigger);
            }
        }
    }

    void PlatformDesktopGLFW::Shutdown()
    {
        if (m_Window != nullptr)
        {
            glfwDestroyWindow(m_Window);
            // glfwTerminate();
            m_Window = nullptr;
            std::cout << "Close Window\n";
        }
    }

    void PlatformDesktopGLFW::Clear(Color color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    Sizei PlatformDesktopGLFW::GetWindowSize() const
    {
        return Sizei(m_ViewPortWidth, m_ViewPortHeight);
    }

    bool PlatformDesktopGLFW::IsWindowResized()
    {
        if (m_isResized)
        {
            m_isResized = false; // Reseta a flag após ser lida
            return true;
        }
        return false;
    }

    double PlatformDesktopGLFW::GetTime() const
    {
        return glfwGetTime();
    }


    void PlatformDesktopGLFW::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        PlatformDesktopGLFW* platform = static_cast<PlatformDesktopGLFW*>(glfwGetWindowUserPointer(window));

        if (platform)
        {
            platform->OnFramebufferResize(width, height);
        }
    }

    void PlatformDesktopGLFW::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key >= 0 && key < Keyboard::keyCount)
        {
            if (action == GLFW_PRESS)
            {
                Input::GetInstance().UpdateKeyboardCurrentState(key, true);
            }
            else if (action == GLFW_RELEASE)
            {
                Input::GetInstance().UpdateKeyboardCurrentState(key, false);
            }
        }
    }

    void PlatformDesktopGLFW::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button >= 0 && button < Mouse::mouseButtonsCount)
        {
            if (action == GLFW_PRESS)
            {
                Input::GetInstance().UpdateMouseCurrentState(button, true);
            }
            else if (action == GLFW_RELEASE)
            {
                Input::GetInstance().UpdateMouseCurrentState(button, false);
            }
        }
    }

    void PlatformDesktopGLFW::MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Vector2i position{static_cast<int>(xpos), static_cast<int>(ypos)};
        Input::GetInstance().UpdateMousePosition(position);
    }

    void PlatformDesktopGLFW::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Input::GetInstance().UpdateMouseWhellMove(static_cast<float>(yoffset));
    }

    void PlatformDesktopGLFW::OnFramebufferResize(int width, int height)
    {
        glViewport(0, 0, width, height);
        m_ViewPortWidth = width;
        m_ViewPortHeight = height;
        m_isResized = true;
    }

    void PlatformDesktopGLFW::JoystickCallback(int jid, int event)
    {

        if (event == GLFW_CONNECTED)
        {
            if (jid < Gamepad::gamePadCount)
            {
                // Um joystick foi conectado
                std::cout << "INFO: Joystick conectado: " << jid << std::endl;
                Input::GetInstance().RegisterGamePad(jid, glfwGetJoystickName(jid));
            }
        }
        else if (event == GLFW_DISCONNECTED)
        {
            if (jid < Gamepad::gamePadCount)
            {
                // Um joystick foi desconectado
                std::cout << "INFO: Joystick desconectado: " << jid << std::endl;
                Input::GetInstance().UnRegisterGamePad(jid);
            }
        }
    }

} // namespace Fusion
