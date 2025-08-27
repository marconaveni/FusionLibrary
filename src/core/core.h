#ifndef CORE_H
#define CORE_H


#if !defined(FUSION_PLATFORM_WEB)
#define GLFW_INCLUDE_NONE // Disable the standard OpenGL header inclusion on GLFW3
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#else
// Define um tipo vazio para GLFWwindow na web, para evitar erros de compilação
struct GLFWwindow;

#endif

namespace Fusion
{

    class Core
    {
    public:

        // Não permitir instanciar esta classe
        Core() = delete;

        // Chamado pela primeira janela para inicializar a GLFW
        static void Init();

        // Chamado pela última janela para finalizar a GLFW
        static void Shutdown();

        // Processa eventos para todas as janelas. Deve ser chamado no loop principal.
        static void PollEvents();

        static GLFWwindow* s_sharedWindow;

    private:

        friend class Window; // Permite que a Window acesse os métodos privados

        // Métodos para a classe Window registrar/desregistrar sua existência
        static void RegisterWindow();
        static void UnregisterWindow();

        static int s_ActiveWindowCount; // Contador de janelas ativas
        static bool s_IsInitialized;
    };

} // namespace Fusion

#endif