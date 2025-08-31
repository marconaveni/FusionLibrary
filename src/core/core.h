#ifndef CORE_H
#define CORE_H


namespace Fusion
{


    class Window;

    class Core
    {
    public:

        Core(const Core&) = delete;
        Core& operator=(const Core&) = delete; // Deleted copy assignment
        void RegisterWindow();
        void UnregisterWindow();
        static bool HasWindowActive();

    private:

        friend class Window; // Permite que a Window acesse os métodos privados

        static Core& GetInstance();
        Core();
        ~Core();
        bool m_isInitialized = false;
    };

} // namespace Fusion

#endif