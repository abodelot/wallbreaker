#ifndef SOUNDSYSTEM_HPP
#define SOUNDSYSTEM_HPP

#include <string>
#include <SFML/Audio.hpp>

/**
 * Static class for playing sound effects and music
 */
class SoundSystem
{
public:
    /**
     * Control music
     */
    static bool openMusicFromFile(const std::string& path);
    static void playMusic();
    static void stopMusic();
    static void pauseMusic();

    /**
     * Play a sound effect
     */
    static void playSound(const std::string& filename, float pitch = 1.f);

    /**
     * Control music volume
     */
    static void setMusicVolume(int volume);
    static int getMusicVolume();

    /**
     * Control sound effects volume
     */
    static void setSoundVolume(int volume);
    static int getSoundVolume();

    /**
     * Turn music on/off
     */
    static void enableMusic(bool enabled);
    static bool isMusicEnabled();

    /**
     * Turn sound effects on/off
     */
    static void enableSound(bool enabled);
    static bool isSoundEnabled();

    /**
     * Stop music and sound effects
     */
    static void stopAll();

private:
    static const int MAX_SOUNDS = 20;

    static sf::Sound   m_sounds[MAX_SOUNDS];
    static int         m_lastSoundPlayed;
    static sf::Music   m_music;
    static int         m_musicVolume;
    static int         m_soundVolume;
    static bool        m_enableMusic;
    static bool        m_enableSound;
};

#endif // SOUNDSYSTEM_HPP
