#include "SoundSystem.hpp"
#include "Resources.hpp"


sf::Sound SoundSystem::m_sounds[MAX_SOUNDS];
int       SoundSystem::m_last_sound_played = 0;
sf::Music SoundSystem::m_music;
int       SoundSystem::m_music_volume = 100;
int       SoundSystem::m_sound_volume = 100;
bool      SoundSystem::m_enable_music = true;
bool      SoundSystem::m_enable_sound = true;

template <class T>
static inline void clamp(T& value, T min, T max)
{
    if (value > max)
        value = max;
    else if (value < min)
        value = min;
}


bool SoundSystem::openMusicFromFile(const std::string& path)
{
    stopMusic();
    if (m_music.openFromFile(path))
    {
        m_music.setLoop(true);
        m_music.setVolume(m_music_volume);
        return true;
    }
    return false;
}


void SoundSystem::playMusic()
{
    if (m_enable_music)
    {
        m_music.play();
    }
}


void SoundSystem::stopMusic()
{
    m_music.stop();
}


void SoundSystem::pauseMusic()
{
    m_music.pause();
}


void SoundSystem::playSound(const std::string& filename, float pitch)
{
    if (m_enable_sound)
    {
        if (m_last_sound_played == MAX_SOUNDS)
        {
            m_last_sound_played = 0;
        }
        sf::Sound& sound = m_sounds[m_last_sound_played];
        if (sound.getStatus() == sf::Sound::Playing)
        {
            sound.stop();
        }
        sound.setBuffer(Resources::getSoundBuffer(filename));
        sound.setPitch(pitch);
        sound.play();
        ++m_last_sound_played;
    }
}


void SoundSystem::setMusicVolume(int volume)
{
    clamp(volume, 0, 100);
    m_music_volume = volume;
    m_music.setVolume(volume);
}


int SoundSystem::getMusicVolume()
{
    return m_music_volume;
}


void SoundSystem::setSoundVolume(int volume)
{
    clamp(volume, 0, 100);
    m_sound_volume = volume;
    for (int i = 0; i < MAX_SOUNDS; ++i)
    {
        m_sounds[i].setVolume(volume);
    }
}


int SoundSystem::getSoundVolume()
{
    return m_sound_volume;
}


void SoundSystem::enableMusic(bool enabled)
{
    m_enable_music = enabled;
    if (!enabled)
        stopMusic();
    else
        m_music.play();
}


void SoundSystem::enableSound(bool enabled)
{
    m_enable_sound = enabled;
}


bool SoundSystem::isMusicEnabled()
{
    return m_enable_music;
}


bool SoundSystem::isSoundEnabled()
{
    return m_enable_sound;
}


void SoundSystem::stopAll()
{
    for (int i = 0; i < MAX_SOUNDS; ++i)
    {
        if (m_sounds[i].getStatus() == sf::Sound::Playing)
        {
            m_sounds[i].stop();
        }
    }
    stopMusic();
}
