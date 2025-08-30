#include "platform_web.h"

#include <emscripten/emscripten.h>
#include <iostream>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h> // Sim, o GLFW também na web!
#include <glad/glad.h>  // E o GLAD

namespace Fusion
{

    // Construtor e Destrutor
    PlatformWeb::PlatformWeb()
    {
        std::cout << "PlatformWeb: Created! Started" << std::endl;
    }

    PlatformWeb::~PlatformWeb()
    {
        if (m_window)
        {
            glfwDestroyWindow(m_window);
        }
        glfwTerminate();
    }

    void PlatformWeb::Init(const char* title, int width, int height)
    {
        std::cout << "PlatformWeb: Initializing graphics..." << std::endl;
        if (!glfwInit())
        {
            std::cerr << "PlatformWeb: Failed to initialize GLFW" << std::endl;
            return;
        }

        // Dicas para o Emscripten criar um contexto WebGL2
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        m_window = glfwCreateWindow(width, height, title, NULL, NULL);
        m_viewPortWidth = width;
        m_viewPortHeight = height;
        
        if (!m_window)
        {
            std::cerr << "PlatformWeb: Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_window);
        glfwSetWindowUserPointer(m_window, this); // guarda um ponteiro da classe no glfw

        // Agora, o passo crucial: inicializar o GLAD
        if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "PlatformWeb: Failed to initialize GLAD" << std::endl;
            return;
        }

        std::cout << "PlatformWeb: Graphics initialized successfully!" << std::endl;


        // Registra as funções de callback no GLFW
        glfwSetKeyCallback(m_window, KeyCallback);

        emscripten_set_gamepadconnected_callback(NULL, 1, EmscriptenGamepadCallback);
        emscripten_set_gamepaddisconnected_callback(NULL, 1, EmscriptenGamepadCallback);
    }

    void PlatformWeb::PollEventsAndUpdate()
    {
        InputEvents();
        glfwSwapBuffers(m_window);
        // std::cout << "aqui\n";
    }

    void PlatformWeb::InputEvents()
    {
        EmscriptenGamepadEvent gamepadState{};
        int numGamepads = 0;
        if (emscripten_sample_gamepad_data() == EMSCRIPTEN_RESULT_SUCCESS)
        {
            numGamepads = emscripten_get_num_gamepads();
        }

        for (int i = 0; (i < numGamepads) && (i < Gamepad::gamePadCount); i++)
        {
            int result = emscripten_get_gamepad_status(i, &gamepadState);

            if (result == EMSCRIPTEN_RESULT_SUCCESS)
            {
                for (int j = 0; (j < gamepadState.numButtons) && (j < Gamepad::gamePadButtonCount); j++)
                {
                    int button = -1;

                    switch (j)
                    {
                        case 0: button = Gamepad::Button::RIGHT_FACE_DOWN; break;
                        case 1: button = Gamepad::Button::RIGHT_FACE_RIGHT; break;
                        case 2: button = Gamepad::Button::RIGHT_FACE_LEFT; break;
                        case 3: button = Gamepad::Button::RIGHT_FACE_UP; break;
                        case 4: button = Gamepad::Button::LEFT_SHOULDER; break;
                        case 5: button = Gamepad::Button::RIGHT_SHOULDER; break;
                        case 6: button = Gamepad::Button::LEFT_TRIGGER; break;
                        case 7: button = Gamepad::Button::RIGHT_TRIGGER; break;
                        case 8: button = Gamepad::Button::MIDDLE_LEFT; break;
                        case 9: button = Gamepad::Button::MIDDLE_RIGHT; break;
                        case 10: button = Gamepad::Button::LEFT_THUMB_STICK; break;
                        case 11: button = Gamepad::Button::RIGHT_THUMB_STICK; break;
                        case 12: button = Gamepad::Button::LEFT_FACE_UP; break;
                        case 13: button = Gamepad::Button::LEFT_FACE_DOWN; break;
                        case 14: button = Gamepad::Button::LEFT_FACE_LEFT; break;
                        case 15: button = Gamepad::Button::LEFT_FACE_RIGHT; break;
                        default: break;
                    }


                    if (button != -1) // Check for valid button
                    {
                        if (gamepadState.digitalButton[j] == 1)
                        {
                            Input::GetInstance().UpdateGamePadCurrentState(i, button, true);
                            std::cout << "button values: " << button << " | j values: " << j << "\n";
                        }
                        else
                        {
                            Input::GetInstance().UpdateGamePadCurrentState(i, button, false);
                        }
                    }
                }
            }
            for (int j = 0; (j < gamepadState.numAxes) && (j < Gamepad::gamePadAxisCount); j++)
            {
                Input::GetInstance().UpdateGamePadCurrentStateAxis(i, j, gamepadState.axis[j]);
            }
        }
    }

    void PlatformWeb::Clear(Color color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    Sizei PlatformWeb::GetWindowSize() const
    {
        return Sizei(m_viewPortWidth, m_viewPortHeight);
    }

    double PlatformWeb::GetTime() const
    {
        return glfwGetTime();
    }

    void PlatformWeb::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

    EM_BOOL PlatformWeb::EmscriptenGamepadCallback(int eventType, const EmscriptenGamepadEvent* gamepadEvent,
                                                   void* userData)
    {
        if (gamepadEvent->connected && (gamepadEvent->index < Gamepad::gamePadCount))
        {
            std::cout << "Controle detectado: " << gamepadEvent->id << "\n";
            Input::GetInstance().RegisterGamePad(gamepadEvent->index, gamepadEvent->id);
        }
        else
        {
            std::cout << "Controle desconectado: " << gamepadEvent->id << "\n";
            Input::GetInstance().UnRegisterGamePad(gamepadEvent->index);
        }
        return 1;
    }

    void PlatformWeb::SetMainLoop(std::function<void()> loop)
    {
        m_mainLoop = loop;
        emscripten_set_main_loop_arg([](void* arg) { static_cast<PlatformWeb*>(arg)->m_mainLoop(); }, this, 0, 1);
    }

} // namespace Fusion