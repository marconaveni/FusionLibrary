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
    }

    void PlatformWeb::PollEventsAndUpdate()
    {
        glfwSwapBuffers(g_window);
        std::cout << "aqui\n";
    }

    void PlatformWeb::Clear(Color color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
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