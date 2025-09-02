#include "audio_device.h"
#include "raudio.h"


namespace Fusion
{

    AudioDevice::AudioDevice()
    {   
    }

    AudioDevice::~AudioDevice()
    {
    }

    void AudioDevice::Init()
    {
        ::InitAudioDevice();
        m_isInitialized = ::IsAudioDeviceReady();
        
    }

    void AudioDevice::Shutdown()
    {
        ::CloseAudioDevice();
        m_isInitialized = ::IsAudioDeviceReady();
    }

    bool AudioDevice::IsReady()
    {
        return m_isInitialized;
    }

    AudioDevice& AudioDevice::GetInstance()
    {
        static AudioDevice instance;
        return instance;
    }


} // namespace Fusion::Audio
