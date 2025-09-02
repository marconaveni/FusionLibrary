#ifndef AUDIO_DEVICE_H
#define AUDIO_DEVICE_H

namespace Fusion
{
    class AudioDevice
    {
    public:

        AudioDevice(const AudioDevice&) = delete;
        AudioDevice& operator=(const AudioDevice&) = delete;

        
        void Init();
        void Shutdown();
        bool IsReady();
        
        static AudioDevice& GetInstance();
    private:

        AudioDevice();
        ~AudioDevice();

        bool m_isInitialized = false;
    };
} // namespace Fusion::Audio

#endif // AUDIO_DEVICE_H