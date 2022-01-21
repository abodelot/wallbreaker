#include "SoundSystem.hpp"
#include "Resources.hpp"


sf::Sound SoundSystem::m_sounds[MAX_SOUNDS];
int SoundSystem::m_lastSoundPlayed = 0;
sf::Music SoundSystem::m_music;
int SoundSystem::m_musicVolume = 100;
int SoundSystem::m_soundVolume = 100;
bool SoundSystem::m_enableMusic = true;
bool SoundSystem::m_enableSound = true;

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
        m_music.setVolume(m_musicVolume);
        return true;
    }
    return false;
}


void SoundSystem::playMusic()
{
    if (m_enableMusic)
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
    if (m_enableSound)
    {
        if (m_lastSoundPlayed == MAX_SOUNDS)
        {
            m_lastSoundPlayed = 0;
        }
        sf::Sound& sound = m_sounds[m_lastSoundPlayed];
        if (sound.getStatus() == sf::Sound::Playing)
        {
            sound.stop();
        }
        sound.setBuffer(Resources::getSoundBuffer(filename));
        sound.setPitch(pitch);
        sound.play();
        ++m_lastSoundPlayed;
    }
}


void SoundSystem::setMusicVolume(int volume)
{
    clamp(volume, 0, 100);
    m_musicVolume = volume;
    m_music.setVolume(volume);
}


int SoundSystem::getMusicVolume()
{
    return m_musicVolume;
}


void SoundSystem::setSoundVolume(int volume)
{
    clamp(volume, 0, 100);
    m_soundVolume = volume;
    for (int i = 0; i < MAX_SOUNDS; ++i)
    {
        m_sounds[i].setVolume(volume);
    }
}


int SoundSystem::getSoundVolume()
{
    return m_soundVolume;
}


void SoundSystem::enableMusic(bool enabled)
{
    m_enableMusic = enabled;
    if (!enabled)
        stopMusic();
    else
        m_music.play();
}


void SoundSystem::enableSound(bool enabled)
{
    m_enableSound = enabled;
}


bool SoundSystem::isMusicEnabled()
{
    return m_enableMusic;
}


bool SoundSystem::isSoundEnabled()
{
    return m_enableSound;
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
