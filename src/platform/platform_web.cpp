#include "platform_web.h"
#include <iostream>
#include <emscripten/emscripten.h>

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>  // E o GLAD
#include <GLFW/glfw3.h> // Sim, o GLFW também na web!

namespace Fusion
{

    GLFWwindow *g_window = nullptr;

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

    void PlatformWeb::Init(const char *title, int width, int height)
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

        for (int i = 0; (i < numGamepads) && (i < 4); i++)
        {
            int result = emscripten_get_gamepad_status(i, &gamepadState);

            if (result == EMSCRIPTEN_RESULT_SUCCESS)
            {
                for (int j = 0; (j < gamepadState.numButtons) && (j < 32); j++)
                {
                    // m_gamepadButtonPreviousState[i][j] = m_gamepadButtonCurrentState[i][j];
                    if (gamepadState.digitalButton[j] == true)
                        std::cout << "button " << gamepadState.digitalButton[j] << "\n";
                }
            }
            for (int j = 0; (j < gamepadState.numAxes) && (j < 8); j++)
            {
                std::cout << "axis " << gamepadState.axis[j] << "\n";
            }
        }
    }

    void PlatformWeb::Clear(Color color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    EM_BOOL PlatformWeb::EmscriptenGamepadCallback(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData)
    {
        return 1;
    }

    void PlatformWeb::SetMainLoop(std::function<void()> loop)
    {
        m_MainLoop = loop;
        emscripten_set_main_loop_arg([](void *arg)
                                     {
        // 'arg' é um ponteiro para a nossa instância de PlatformWeb
        static_cast<PlatformWeb*>(arg)->m_MainLoop(); }, this, 0, 1);
    }

}