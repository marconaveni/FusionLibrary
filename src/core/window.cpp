#include "window.h"

#if defined(FUSION_PLATFORM_WEB)
#include "platform_web.h"
#else
#include "platform_desktop_glfw.h"
#endif


#include <chrono> // Para std::chrono::duration
#include <cmath>
#include <iostream>
#include <thread> // Para std::this_thread::sleep_for

#include "camera2d.h"
#include "core.h"
#include "font_data.h"
#include "input.h"
#include "render_texture.h"
#include "renderer.h"
#include "sprite.h"
#include "text.h"

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

    Window::Window()
    {
    }

    Window::~Window()
    {
        Close();
    }

    void Window::InitWindow(const char* title, int width, int height)
    {
#if defined(_WIN32)
        timeBeginPeriod(1);
#endif
        Core::Init();
        Core::RegisterWindow();

        m_renderer = std::make_unique<Renderer>();

#if defined(FUSION_PLATFORM_WEB)
        // Se estivermos compilando para a web, usa a PlatformWeb
        m_Platform = std::make_unique<PlatformWeb>();
#else
        // Senão, usa a plataforma de desktop padrão
        m_platform = std::make_unique<PlatformDesktopGLFW>();
#endif

        m_platform->Init(title, width, height);
        m_renderer->Init(width, height);

        m_defaultProjection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);
        m_defaultFont.LoadFromMemory(DefaultFont::NataSansRegular, DefaultFont::NataSansRegularLen, 32, 255);
        m_time.previous = GetTime();
        m_time.frameCounter = 0;
    }

    void Window::Close()
    {
        if (m_platform->IsWindowActive())
        {
            // a ordem importa descarregamos os recursos de font e render antes de fechar o janela/contexto do opengl
            m_defaultFont.Unload();
            m_renderer->Shutdown();
            m_platform->Shutdown();
            Core::UnregisterWindow();
        }
#if defined(_WIN32)
        timeEndPeriod(1);
#endif
    }

    bool Window::WindowShouldClose()
    {
        return m_platform->WindowShouldClose();
    }

    bool Window::IsWindowResize()
    {
        return m_platform->IsWindowResized();
    }

    Sizei Window::GetWindowSize() const
    {
        return m_platform->GetWindowSize();
    }

    void Window::Clear(Color color)
    {
        m_platform->Clear(color);
    }

    void Window::BeginDrawing()
    {

        // todo time

        m_time.current = GetTime();
        m_time.update = m_time.current - m_time.previous;
        m_time.previous = m_time.current; // Prepara para o próximo quadro

        // Calcula o FPS com base no tempo final e real do quadro
        if (m_time.frame > 0.0)
        {
            m_time.fps = static_cast<int>(1.0 / m_time.frame);
        }


        m_platform->MakeContextCurrent(); 
        if (IsWindowResize())
        {
            Sizei newSize = m_platform->GetWindowSize();
            glm::mat4 newProjection = glm::ortho(0.0f, static_cast<float>(newSize.width), static_cast<float>(newSize.height), 0.0f);
            m_renderer->SetProjection(newProjection);
            m_defaultProjection = newProjection;
        }
        m_renderer->BeginRender();
    }

    void Window::Draw(const Sprite& sprite, Shader* customShader)
    {
        m_renderer->DrawTexture(sprite, customShader);
    }

    void Window::Draw(Text& text, Shader* customShader)
    {
        m_renderer->DrawText(text, customShader);
    }

    void Window::DrawRectangle(int x, int y, int width, int height, Color color)
    {
        m_renderer->DrawRectangle(x, y, width, height, color);
    }

    void Window::DrawCircle(int centerX, int centerY, float radius, Color color)
    {
        m_renderer->DrawCircle(Vector2f({(float)centerX, (float)centerY}), radius, color);
    }

    void Window::DrawCircle(Vector2f center, float radius, Color color)
    {
        m_renderer->DrawCircle(center, radius, color);
    }

    void Window::DrawCircleLines(int centerX, int centerY, float radius, Color color)
    {
        m_renderer->DrawCircleLines(Vector2f({(float)centerX, (float)centerY}), radius, color);
    }

    void Window::DrawCircleLines(Vector2f center, float radius, Color color)
    {
        m_renderer->DrawCircleLines(center, radius, color);
    }

    void Window::DrawTriangle(Vector2f v1, Vector2f v2, Vector2f v3, Color color)
    {
        m_renderer->DrawTriangle(v1, v2, v3, color);
    }

    void Window::DrawLine(Vector2f startPos, Vector2f endPos, float thick, Color color)
    {
        m_renderer->DrawLine(startPos, endPos, thick, color);
    }

    void Window::DrawRectangleLines(int x, int y, int width, int height, float thick, Color color)
    {
        m_renderer->DrawRectangleLines(x, y, width, height, thick, color);
    }

    void Window::WaitTime(double seconds)
    {

        if (seconds < 0)
        {
            return; // checagem de segurança
        }

        double destinationTime = GetTime() + seconds;

        double sleepSeconds = seconds - seconds * 0.05;
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepSeconds * 1000)));

        while (GetTime() < destinationTime)
        {
            std::this_thread::yield(); // cede CPU pra não fritar
        }
    }

    void Window::EndDrawing()
    {
        m_renderer->EndRender();

        m_time.current = GetTime();
        m_time.draw = m_time.current - m_time.previous;
        m_time.previous = m_time.current;

        m_time.frame = m_time.update + m_time.draw;

        if (m_time.frame < m_time.target)
        {

            WaitTime(m_time.target - m_time.frame);

            m_time.current = GetTime();
            double waitTime = m_time.current - m_time.previous;
            m_time.previous = m_time.current;

            m_time.frame += waitTime;
        }

        m_platform->PollEventsAndUpdate();

        m_time.frameCounter++;
    }

    void Window::BeginScissorMode(int x, int y, int width, int height)
    {
        // 1. Desenha tudo que estava no lote atual ANTES de ativar o Scissor.
        m_renderer->EndRender();

        // 2. Converte a coordenada Y do sistema "top-left" para "bottom-left" do OpenGL.
        const int openglY = GetWindowSize().height - (y + height);

        // 3. Ativa o teste Scissor com as coordenadas corretas.
        m_renderer->BeginScissorMode(x, openglY, width, height);
    }

    void Window::EndScissorMode()
    {
        // 1. Desenha tudo que estava no lote DENTRO do modo Scissor.
        m_renderer->EndRender();
        m_renderer->EndScissorMode();
    }

    void Window::BeginTextureMode(const RenderTexture& target)
    {
        m_renderer->EndRender(); // Flush antes de mudar o alvo

        glBindFramebuffer(GL_FRAMEBUFFER, target.GetFboId());


        const auto size = target.GetTexture()->GetSize();
        glViewport(0, 0, size.width, size.height);

        //std::cout << size.width << " x " << size.height << "\n";

        // Define uma nova projeção ortográfica para o tamanho da textura
        glm::mat4 textureProjection = glm::ortho(0.0f, (float)size.width, (float)size.height, 0.0f);
        m_renderer->SetProjection(textureProjection);

        // Limpa a textura com uma cor (opcional, mas recomendado)
        // Usamos m_Platform->Clear para manter a abstração
        m_platform->Clear({0.0f, 0.0f, 0.0f, 0.0f});
    }

    void Window::EndTextureMode()
    {
        m_renderer->EndRender(); // Flush no que foi desenhado na textura

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Volta para a tela

        const Sizei size = GetWindowSize();
        glViewport(0, 0, size.width, size.height);
        m_renderer->SetProjection(m_defaultProjection); // Restaura a projeção da janela
    }

    void Window::BeginMode2D(const Camera2D& camera)
    {
        m_renderer->EndRender(); // Desenha qualquer coisa pendente no modo padrão
        m_renderer->SetViewMatrix(camera.GetViewMatrix());
    }

    void Window::EndMode2D()
    {
        m_renderer->EndRender(); // Desenha qualquer coisa pendente no modo 2D
        m_renderer->ResetViewMatrix();
    }

    void Window::BeginBlendMode(BlendMode mode)
    {
        m_renderer->BeginBlendMode(mode);
    }

    void Window::EndBlendMode()
    {
        m_renderer->EndBlendMode();
    }

    double Window::GetFrameTime() const
    {
        return m_time.frame;
    }

    int Window::GetFPS()
    {
        // using same code average fps used in raylib 
        int fps = 0;

        constexpr int fpsCaptureFramesCount = 30;   // 30 captures;
        constexpr float fpsAverageTimeSeconds = 0.5f; // 500 milliseconds;
        constexpr float fpsStep = (fpsAverageTimeSeconds / fpsCaptureFramesCount);

        static int index = 0;
        static float history[fpsCaptureFramesCount] = {};
        static float average = 0; 
        static float last = 0;
        float fpsFrame = GetFrameTime();

        // if we reset the window, reset the FPS info
        if (m_time.frameCounter == 0)
        {
            average = 0;
            last = 0;
            index = 0;

            for (int i = 0; i < fpsCaptureFramesCount; i++)
            {
                history[i] = 0;
            }
        }

        if (fpsFrame == 0)
        {
            return 0;
        }

        if ((GetTime() - last) > fpsStep)
        {
            last = (float)GetTime();
            index = (index + 1) % fpsCaptureFramesCount;
            average -= history[index];
            history[index] = fpsFrame / fpsCaptureFramesCount;
            average += history[index];
        }

        fps = (int)roundf(1.0f / average);

        return fps;
    }

    double Window::GetTime() const
    {
        return m_platform->GetTime();
    }

    void Window::SetTargetFPS(int fps)
    {
        if (fps > 0)
        {
            m_time.target = 1.0 / static_cast<double>(fps);
        }
        else
        {
            m_time.target = 0.0;
        }
    }

    Font& Window::GetDefaultFont()
    {
        return m_defaultFont;
    }


    void Window::SetMainLoop(std::function<void()> loop)
    {
        // A Window tem a definição completa, então ela pode chamar o método.
        m_platform->SetMainLoop(std::move(loop));
    }

} // namespace Fusion
