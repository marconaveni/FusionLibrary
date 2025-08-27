#include "platform_web.h"

#include <emscripten/emscripten.h>
#include <iostream>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h> // Sim, o GLFW também na web!
#include <glad/glad.h>  // E o GLAD

namespace Fusion
{

    GLFWwindow* g_window = nullptr;

    // Construtor e Destrutor
    PlatformWeb::PlatformWeb()
    {
        std::cout << "PlatformWeb: Created! Started" << std::endl;
    }
    PlatformWeb::~PlatformWeb()
    {
        if (g_window)
        {
            glfwDestroyWindow(g_window);
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

        g_window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!g_window)
        {
            std::cerr << "PlatformWeb: Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(g_window);

        // Agora, o passo crucial: inicializar o GLAD
        if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "PlatformWeb: Failed to initialize GLAD" << std::endl;
            return;
        }

        std::cout << "PlatformWeb: Graphics initialized successfully!" << std::endl;

        emscripten_set_gamepadconnected_callback(NULL, 1, EmscriptenGamepadCallback);
        emscripten_set_gamepaddisconnected_callback(NULL, 1, EmscriptenGamepadCallback);
    }

    void PlatformWeb::PollEventsAndUpdate()
    {
        InputEvents();
        glfwSwapBuffers(g_window);
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
                        case 0 : button = Gamepad::Button::GAMEPAD_BUTTON_RIGHT_FACE_DOWN; break;
                        case 1 : button = Gamepad::Button::GAMEPAD_BUTTON_RIGHT_FACE_RIGHT; break;
                        case 2 : button = Gamepad::Button::GAMEPAD_BUTTON_RIGHT_FACE_LEFT; break;
                        case 3 : button = Gamepad::Button::GAMEPAD_BUTTON_RIGHT_FACE_UP; break;
                        case 4 : button = Gamepad::Button::GAMEPAD_BUTTON_LEFT_TRIGGER_1; break;
                        case 5 : button = Gamepad::Button::GAMEPAD_BUTTON_RIGHT_TRIGGER_1; break;
                        case 6 : button = Gamepad::Button::GAMEPAD_BUTTON_LEFT_TRIGGER_2; break;
                        case 7 : button = Gamepad::Button::GAMEPAD_BUTTON_RIGHT_TRIGGER_2; break;
                        case 8 : button = Gamepad::Button::GAMEPAD_BUTTON_MIDDLE_LEFT; break;
                        case 9 : button = Gamepad::Button::GAMEPAD_BUTTON_MIDDLE_RIGHT; break;
                        case 10: button = Gamepad::Button::GAMEPAD_BUTTON_LEFT_THUMB; break;
                        case 11: button = Gamepad::Button::GAMEPAD_BUTTON_RIGHT_THUMB; break;
                        case 12: button = Gamepad::Button::GAMEPAD_BUTTON_LEFT_FACE_UP; break;
                        case 13: button = Gamepad::Button::GAMEPAD_BUTTON_LEFT_FACE_DOWN; break;
                        case 14: button = Gamepad::Button::GAMEPAD_BUTTON_LEFT_FACE_LEFT; break;
                        case 15: button = Gamepad::Button::GAMEPAD_BUTTON_LEFT_FACE_RIGHT; break;
                        default: break;
                    }


                    if (button + 1 != 0) // Check for valid button
                    {
                        if (gamepadState.digitalButton[j] == 1)
                        {
                            Input::GetInstance().UpdateGamePadCurrentState(i, button, true);
                            std::cout << "button " << j << "\n";
                        }
                    }
                    else
                    {
                        Input::GetInstance().UpdateGamePadCurrentState(i, button, false);
                    }
                }
            }
            for (int j = 0; (j < gamepadState.numAxes) && (j < Gamepad::gamePadAxisCount); j++)
            {
                Input::GetInstance().UpdateGamePadCurrentStateAxis(i, j, gamepadState.numAxes);
                if (gamepadState.axis[j] > 0.1f || gamepadState.axis[j] < -0.1f)
                    std::cout << "axis " << gamepadState.axis[j] << "\n";
            }
        }
    }

    void PlatformWeb::Clear(Color color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    EM_BOOL PlatformWeb::EmscriptenGamepadCallback(int eventType, const EmscriptenGamepadEvent* gamepadEvent, void* userData)
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
        m_MainLoop = loop;
        emscripten_set_main_loop_arg([](void* arg) { static_cast<PlatformWeb*>(arg)->m_MainLoop(); }, this, 0, 1);
    }

} // namespace Fusion