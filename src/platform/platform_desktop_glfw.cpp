#include "platform_desktop_glfw.h"

#include <chrono> // Para std::chrono::duration
#include <cmath>
#include <iostream>
#include <thread> // Para std::this_thread::sleep_for

#include "core.h"
#include "fusion_math.h"
#include "input.h"

// note que isso é usado para evitar chamar bibliotecas do windows que podem causar muitos conflitos
#if defined(_WIN32)
extern "C"
{
    __declspec(dllimport) unsigned int __stdcall timeBeginPeriod(unsigned int uPeriod);
    __declspec(dllimport) unsigned int __stdcall timeEndPeriod(unsigned int uPeriod);
}
#endif

namespace Fusion
{

    void PlatformDesktopGLFW::Init(const char* title, int width, int height)
    {
#if defined(_WIN32)
        timeBeginPeriod(1);
#endif

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
        glfwSetWindowUserPointer(m_Window, this);

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

        m_PreviousTime = glfwGetTime();
    }

    bool PlatformDesktopGLFW::IsWindowActive()
    {
        return m_Window != nullptr;
    }

    void PlatformDesktopGLFW::MakeContextCurrent()
    {
        glfwMakeContextCurrent(m_Window);
    }

    bool PlatformDesktopGLFW::WindowShouldClose()
    {
        return glfwWindowShouldClose(m_Window);
    }

    void PlatformDesktopGLFW::PollEventsAndUpdate()
    {

        // Mede o tempo gasto na lógica do jogo e no desenho do quadro anterior
        m_CurrentTime = glfwGetTime();
        double workTime = m_CurrentTime - m_PreviousTime;

        // Se um FPS alvo está definido e o quadro foi rápido demais...
        if (m_targetFrameTime > 0.0 && workTime < m_targetFrameTime)
        {
            double waitTime = m_targetFrameTime - workTime;

            // Ponto no tempo que queremos atingir
            double destinationTime = m_PreviousTime + m_targetFrameTime;

            // Espera híbrida (sleep + busy-wait) para alta precisão
            if (waitTime > 0.0)
            {
                // Tenta dormir pela maior parte do tempo, deixando ~1ms para a espera ocupada
                double sleepSeconds = waitTime - 0.001;
                if (sleepSeconds > 0.0)
                {
                    // std::cout << sleepSeconds << "\n";
                    std::this_thread::sleep_for(std::chrono::duration<double>(sleepSeconds)); // comentando esse bloco if o limitador de quadros funcionou bem
                }

                // Espera ocupada (busy-wait) para o tempo restante, garantindo precisão
                while (glfwGetTime() < destinationTime) {}
            }
        }

        // Troca os buffers APÓS a espera
        glfwSwapBuffers(m_Window);

        // Atualiza o estado do input para o próximo quadro
        InputEvents();

        // Processa os eventos de janela (como fechar)
        glfwPollEvents();

        // Mede o tempo final do quadro, incluindo a espera
        m_CurrentTime = glfwGetTime();
        m_FrameTime = m_CurrentTime - m_PreviousTime;
        m_PreviousTime = m_CurrentTime; // Prepara para o próximo quadro

        // Calcula o FPS com base no tempo final e real do quadro
        if (m_FrameTime > 0.0)
        {
            m_Fps = static_cast<int>(std::round(1.0 / m_FrameTime));
        }
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
                    Input::GetInstance().UpdateGamePadCurrentState(i, j, (state.buttons[j] == GLFW_PRESS));
                }
                // Atualiza o estado dos eixos
                for (int j = 0; j < Gamepad::gamePadAxisCount; j++)
                {
                    Input::GetInstance().UpdateGamePadCurrentStateAxis(i, j, state.axes[j]);
                }
            }
        }
    }

    void PlatformDesktopGLFW::Shutdown()
    {
#if defined(_WIN32)
        timeEndPeriod(1);
#endif

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

    void PlatformDesktopGLFW::UpdateTime()
    {

        ////////////////////////////  todo: isso deve ser removido depois
    }

    float PlatformDesktopGLFW::GetFrameTime() const
    {
        return m_FrameTime;
    }

    float PlatformDesktopGLFW::GetTime() const
    {
        return glfwGetTime();
    }

    int PlatformDesktopGLFW::GetFPS() const
    {
        return m_Fps;
    }

    void PlatformDesktopGLFW::SetTargetFPS(int fps)
    {

        if (fps > 0)
        {
            // Desativa o VSync para usar nosso controle manual
            glfwSwapInterval(0);
            m_targetFrameTime = 1.0 / static_cast<double>(fps);
        }
        else
        {
            // Ativa o VSync novamente
            glfwSwapInterval(1);
            m_targetFrameTime = 0.0;
        }
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
        PlatformDesktopGLFW* platform = static_cast<PlatformDesktopGLFW*>(glfwGetWindowUserPointer(window));
        if (platform && key >= 0 && key < Keyboard::keyCount)
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
        PlatformDesktopGLFW* platform = static_cast<PlatformDesktopGLFW*>(glfwGetWindowUserPointer(window));
        if (platform && button >= 0 && button < Mouse::mouseButtonsCount)
        {
            if (action == GLFW_PRESS)
            {
                Input::GetInstance().UpdateMouseCurrentState(button, true);
                // platform->m_mouseCurrentState[button] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                Input::GetInstance().UpdateMouseCurrentState(button, false);
                // platform->m_mouseCurrentState[button] = false;
            }
        }
    }

    void PlatformDesktopGLFW::MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        PlatformDesktopGLFW* platform = static_cast<PlatformDesktopGLFW*>(glfwGetWindowUserPointer(window));
        if (platform)
        {
            Vector2i position{static_cast<int>(xpos), static_cast<int>(ypos)};

            Input::GetInstance().UpdateMousePosition(position);
        }
    }

    void PlatformDesktopGLFW::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        PlatformDesktopGLFW* platform = static_cast<PlatformDesktopGLFW*>(glfwGetWindowUserPointer(window));
        if (platform)
        {
            Input::GetInstance().UpdateMouseWhellMove(static_cast<float>(yoffset));
        }
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
