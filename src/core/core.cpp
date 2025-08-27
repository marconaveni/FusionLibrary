#include "core.h"

#include <iostream>

namespace Fusion
{

    int Core::s_ActiveWindowCount = 0;
    bool Core::s_IsInitialized = false;
    GLFWwindow* Core::s_sharedWindow = nullptr;

    void Core::Init()
    {
        // Só inicializa a GLFW uma vez
        if (!s_IsInitialized)
        {

#if !defined(FUSION_PLATFORM_WEB)
            if (glfwInit())
            {
                s_IsInitialized = true;
                std::cout << "INFO::CORE::GLFW inicializada.\n";
            }
#endif
        }
    }

    void Core::Shutdown()
    {
        // Só finaliza a GLFW se ela foi inicializada
        if (s_IsInitialized)
        {
#if !defined(FUSION_PLATFORM_WEB)
            glfwTerminate();
#endif
            s_IsInitialized = false;
            std::cout << "INFO::CORE::GLFW finalizada.\n";
        }
    }

    void Core::PollEvents()
    {
#if !defined(FUSION_PLATFORM_WEB)
        glfwPollEvents();
#endif
    }

    void Core::RegisterWindow()
    {
        s_ActiveWindowCount++;
    }

    void Core::UnregisterWindow()
    {
        s_ActiveWindowCount--;

        // Se for a última janela a ser fechada, desliga a GLFW
        if (s_ActiveWindowCount == 0)
        {
            Shutdown();
        }
    }

} // namespace Fusion
