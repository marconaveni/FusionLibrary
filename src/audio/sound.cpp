#include "sound.h"

#include <memory> // Para std::make_unique

#include "raudio.h"


namespace Fusion
{


    struct Sound::SoundData
    {
        ::Sound SoundStream{};
        bool isReady = false;
    };


    Sound::Sound()
    {
        m_data = std::make_unique<SoundData>();
    }

    Sound::~Sound()
    {
        Unload();
    }

    bool Sound::LoadFromFile(std::string_view path)
    {
        Unload();
        m_data->SoundStream = ::LoadSound(path.data());
        m_data->isReady = ::IsSoundValid(m_data->SoundStream);
        return m_data->isReady;
    }

    void Sound::Play()
    {
        ::PlaySound(m_data->SoundStream);
    }

    void Sound::Stop()
    {
        ::StopSound(m_data->SoundStream);
    }

    void Sound::Pause()
    {
        ::PauseSound(m_data->SoundStream);
    }

    void Sound::Resume()
    {
        ::ResumeSound(m_data->SoundStream);
    }

    void Sound::SetVolume(float volume)
    {
        ::SetSoundVolume(m_data->SoundStream, volume);
    }

    bool Sound::IsPlaying() const
    {
        return ::IsSoundPlaying(m_data->SoundStream);
    }

    void Sound::Unload()
    {
        if (m_data->isReady)
        {
            ::UnloadSound(m_data->SoundStream);
        }
    }

} // namespace Fusion::Audio
