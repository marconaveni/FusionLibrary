#include "platform_desktop_glfw.h"
#include <iostream>
#include <cmath>
#include <cstring> // Para memset
#include <thread>  // Para std::this_thread::sleep_for
#include <chrono>  // Para std::chrono::duration

struct WindowStatus
{
    int viewPortWidth;
    int viewPortHeight;
};

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

    int Core::s_ActiveWindowCount = 0;
    bool Core::s_IsInitialized = false;
    GLFWwindow *Core::s_sharedWindow = nullptr;

    void Core::Init()
    {
        // Só inicializa a GLFW uma vez
        if (!s_IsInitialized)
        {
            if (glfwInit())
            {
                s_IsInitialized = true;
                std::cout << "INFO::CORE::GLFW inicializada.\n";
            }
        }
    }

    void Core::Shutdown()
    {
        // Só finaliza a GLFW se ela foi inicializada
        if (s_IsInitialized)
        {
            glfwTerminate();
            s_IsInitialized = false;
            std::cout << "INFO::CORE::GLFW finalizada.\n";
        }
    }

    void Core::PollEvents()
    {
        glfwPollEvents();
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

    void PlatformDesktopGLFW::Init(const char *title, int width, int height)
    {
#if defined(_WIN32)
        timeBeginPeriod(1);
#endif

        // glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow *shareWindow = nullptr;
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

        // --- INICIALIZAÇÃO DO INPUT ---
        // Zera todos os arrays de estado
        memset(m_keyCurrentState, 0, sizeof(m_keyCurrentState));
        memset(m_keyPreviousState, 0, sizeof(m_keyPreviousState));
        memset(m_mouseCurrentState, 0, sizeof(m_mouseCurrentState));
        memset(m_mousePreviousState, 0, sizeof(m_mousePreviousState));
        memset(m_gamepadReady, 0, sizeof(m_gamepadReady));
        memset(m_gamepadAxisState, 0, sizeof(m_gamepadAxisState));
        memset(m_gamepadButtonCurrentState, 0, sizeof(m_gamepadButtonCurrentState));
        memset(m_gamepadButtonPreviousState, 0, sizeof(m_gamepadButtonPreviousState));
        m_mouseWheelMove = 0.0f;
        m_previousMouseWheelMove = 0.0f;

        // Registra as funções de callback no GLFW
        glfwSetKeyCallback(m_Window, KeyCallback);
        glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
        glfwSetCursorPosCallback(m_Window, MouseCursorPosCallback);
        glfwSetScrollCallback(m_Window, MouseScrollCallback);
        glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
        glfwSetJoystickCallback(JoystickCallback); // Registra o callback de conexão do joystick

        // Verifica quais gamepads já estão conectados no início
        for (int i = 0; i < MAX_GAMEPADS; i++)
        {
            if (glfwJoystickPresent(i))
            {
                m_gamepadReady[i] = true;
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
                while (glfwGetTime() < destinationTime)
                {
                }
            }
        }

        // Troca os buffers APÓS a espera
        glfwSwapBuffers(m_Window);

        // Atualiza o estado do input para o próximo quadro
        InputEvents();

        // Processa os eventos de janela (como fechar)
        // glfwPollEvents();

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
        // Atualiza o estado anterior das teclas e botões
        for (int i = 0; i < MAX_KEYS; i++)
        {
            m_keyPreviousState[i] = m_keyCurrentState[i];
        }

        for (int i = 0; i < MAX_MOUSE_BUTTONS; i++)
        {
            m_mousePreviousState[i] = m_mouseCurrentState[i];
        }

        // Reseta o movimento da roda do mouse
        m_mouseWheelMove = 0.0f;

        for (int i = 0; i < MAX_GAMEPADS; i++)
        {
            if (m_gamepadReady[i] && glfwJoystickIsGamepad(i))
            {
                // Copia o estado atual para o anterior
                for (int j = 0; j < MAX_GAMEPAD_BUTTONS; j++)
                {
                    m_gamepadButtonPreviousState[i][j] = m_gamepadButtonCurrentState[i][j];
                }
            }

            GLFWgamepadstate state;
            if (glfwGetGamepadState(i, &state))
            {
                // Atualiza o estado dos botões
                for (int j = 0; j < MAX_GAMEPAD_BUTTONS; j++)
                {
                    m_gamepadButtonCurrentState[i][j] = (state.buttons[j] == GLFW_PRESS);
                }
                // Atualiza o estado dos eixos
                for (int j = 0; j < MAX_GAMEPAD_AXES; j++)
                {
                    m_gamepadAxisState[i][j] = state.axes[j];
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

        ////////////////////////////
    }

    float PlatformDesktopGLFW::GetFrameTime() const
    {
        return m_FrameTime;
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

    bool PlatformDesktopGLFW::IsKeyPressed(int key) const
    {
        return m_keyCurrentState[key] && !m_keyPreviousState[key];
    }

    bool PlatformDesktopGLFW::IsKeyDown(int key) const
    {
        return m_keyCurrentState[key];
    }

    bool PlatformDesktopGLFW::IsKeyReleased(int key) const
    {
        return !m_keyCurrentState[key] && m_keyPreviousState[key];
    }

    bool PlatformDesktopGLFW::IsMouseButtonPressed(int button) const
    {
        return m_mouseCurrentState[button] && !m_mousePreviousState[button];
    }

    bool PlatformDesktopGLFW::IsMouseButtonDown(int button) const
    {
        return m_mouseCurrentState[button];
    }

    bool PlatformDesktopGLFW::IsMouseButtonReleased(int button) const
    {
        return !m_mouseCurrentState[button] && m_mousePreviousState[button];
    }

    Vector2f PlatformDesktopGLFW::GetMousePosition() const
    {
        return m_mousePosition;
    }

    float PlatformDesktopGLFW::GetMouseWheelMove() const
    {
        return m_mouseWheelMove;
    }

    void PlatformDesktopGLFW::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        PlatformDesktopGLFW *platform = static_cast<PlatformDesktopGLFW *>(glfwGetWindowUserPointer(window));

        if (platform)
        {
            platform->OnFramebufferResize(width, height);
        }
    }

    void PlatformDesktopGLFW::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        PlatformDesktopGLFW *platform = static_cast<PlatformDesktopGLFW *>(glfwGetWindowUserPointer(window));
        if (platform && key >= 0 && key < MAX_KEYS)
        {
            if (action == GLFW_PRESS)
                platform->m_keyCurrentState[key] = true;
            else if (action == GLFW_RELEASE)
                platform->m_keyCurrentState[key] = false;
        }
    }

    void PlatformDesktopGLFW::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
    {
        PlatformDesktopGLFW *platform = static_cast<PlatformDesktopGLFW *>(glfwGetWindowUserPointer(window));
        if (platform && button >= 0 && button < MAX_MOUSE_BUTTONS)
        {
            if (action == GLFW_PRESS)
                platform->m_mouseCurrentState[button] = true;
            else if (action == GLFW_RELEASE)
                platform->m_mouseCurrentState[button] = false;
        }
    }

    void PlatformDesktopGLFW::MouseCursorPosCallback(GLFWwindow *window, double xpos, double ypos)
    {
        PlatformDesktopGLFW *platform = static_cast<PlatformDesktopGLFW *>(glfwGetWindowUserPointer(window));
        if (platform)
        {
            platform->m_mousePosition.x = (float)xpos;
            platform->m_mousePosition.y = (float)ypos;
        }
    }

    void PlatformDesktopGLFW::MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        PlatformDesktopGLFW *platform = static_cast<PlatformDesktopGLFW *>(glfwGetWindowUserPointer(window));
        if (platform)
        {
            platform->m_mouseWheelMove = (float)yoffset;
        }
    }

    void PlatformDesktopGLFW::OnFramebufferResize(int width, int height)
    {
        glViewport(0, 0, width, height);
        m_ViewPortWidth = width;
        m_ViewPortHeight = height;
        m_isResized = true;
    }

    bool PlatformDesktopGLFW::IsGamepadAvailable(int gamepad) const
    {
        return (gamepad < MAX_GAMEPADS) && m_gamepadReady[gamepad];
    }

    const char *PlatformDesktopGLFW::GetGamepadName(int gamepad) const
    {
        if (IsGamepadAvailable(gamepad))
            return glfwGetGamepadName(gamepad);
        return nullptr;
    }

    bool PlatformDesktopGLFW::IsGamepadButtonPressed(int gamepad, int button) const
    {
        return IsGamepadAvailable(gamepad) && m_gamepadButtonCurrentState[gamepad][button] && !m_gamepadButtonPreviousState[gamepad][button];
    }

    bool PlatformDesktopGLFW::IsGamepadButtonDown(int gamepad, int button) const
    {
        return IsGamepadAvailable(gamepad) && m_gamepadButtonCurrentState[gamepad][button];
    }

    bool PlatformDesktopGLFW::IsGamepadButtonReleased(int gamepad, int button) const
    {
        return IsGamepadAvailable(gamepad) && !m_gamepadButtonCurrentState[gamepad][button] && m_gamepadButtonPreviousState[gamepad][button];
    }

    float PlatformDesktopGLFW::GetGamepadAxisMovement(int gamepad, int axis) const
    {
        float value = 0.0f;
        if (IsGamepadAvailable(gamepad) && axis < MAX_GAMEPAD_AXES)
        {
            value = m_gamepadAxisState[gamepad][axis];
            // Aplica uma "deadzone" para evitar movimentos fantasmas
            if (fabs(value) < 0.1f)
                value = 0.0f;
        }
        return value;
    }

    void PlatformDesktopGLFW::JoystickCallback(int jid, int event)
    {

        if (event == GLFW_CONNECTED)
        {
            if (jid < MAX_GAMEPADS)
            {
                // Um joystick foi conectado
                // A implementação real deve atualizar o estado interno
                std::cout << "INFO: Joystick conectado: " << jid << std::endl;
            }
        }
        else if (event == GLFW_DISCONNECTED)
        {
            if (jid < MAX_GAMEPADS)
            {
                // Um joystick foi desconectado
                std::cout << "INFO: Joystick desconectado: " << jid << std::endl;
            }
        }
    }

}
