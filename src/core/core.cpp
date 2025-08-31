#include "core.h"

#include <iostream>

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

    Core::Core()
    {
#if defined(_WIN32)
        timeBeginPeriod(1);
#endif
    }

    Core::~Core()
    {
#if defined(_WIN32)
        timeEndPeriod(1);
#endif
    }

    bool Core::HasWindowActive()
    {
        return GetInstance().m_isInitialized;
    }

    void Core::RegisterWindow()
    {
        if (!m_isInitialized)
        {
            m_isInitialized = true;
            std::cout << "INFO::CORE::GLFW(WEB) inicializada.\n";
        }
    }

    void Core::UnregisterWindow()
    {
        if (m_isInitialized)
        {
            m_isInitialized = false;
            std::cout << "INFO::CORE::WINDOW finalizada.\n";
        }
    }

    Core& Core::GetInstance()
    {
        static Core instance;
        return instance;
    }


} // namespace Fusion
