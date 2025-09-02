#include "music.h"

#include <memory> // Para std::make_unique

#include "raudio.h"


namespace Fusion
{


    struct Music::MusicData
    {
        ::Music musicStream {};
        bool isReady = false;
    };


    Music::Music()
    {
        m_data = std::make_unique<MusicData>();
    }

    Music::~Music()
    {
        Unload();
    }

    bool Music::LoadFromFile(std::string_view path)
    {
        Unload();
        m_data->musicStream = ::LoadMusicStream(path.data());
        m_data->isReady = ::IsMusicValid(m_data->musicStream);
        return m_data->isReady;
    }

    void Music::Play()
    {
        ::PlayMusicStream(m_data->musicStream);
    }

    void Music::Stop()
    {
        ::StopMusicStream(m_data->musicStream);
    }

    void Music::Pause()
    {
        ::PauseMusicStream(m_data->musicStream);
    }

    void Music::Resume()
    {
        ::ResumeMusicStream(m_data->musicStream);
    }

    void Music::UpdateStream()
    {
        ::UpdateMusicStream(m_data->musicStream);
    }

    void Music::SetVolume(float volume)
    {
        ::SetMusicVolume(m_data->musicStream, volume);
    }

    bool Music::IsPlaying() const
    {
        return ::IsMusicStreamPlaying(m_data->musicStream);
    }

    float Music::GetDuration() const
    {
        return ::GetMusicTimeLength(m_data->musicStream);
    }

    float Music::GetTimePlayed() const
    {
        return ::GetMusicTimePlayed(m_data->musicStream);
    }

    void Music::Unload()
    {
        if (m_data->isReady)
        {
            ::UnloadMusicStream(m_data->musicStream);
        }
    }

} // namespace Fusion::Audio
