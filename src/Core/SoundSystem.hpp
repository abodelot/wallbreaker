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
	 * Set/Get current background music
	 */
	static void setMusic(const std::string& music_name);
	static const std::string& getMusic();

	/**
	 * Control music
	 */
	static void playMusic();
	static void stopMusic();
	static void pauseMusic();

	/**
	 * Play a sound effect
	 */
	static void playSound(const std::string& sound_name, float pitch = 1.f);

	// Music volume
	static void setMusicVolume(int volume);
	static inline int getMusicVolume() { return m_music_volume; }

	// Sound effects volume
	static void setSoundVolume(int volume);
	static inline int getSoundVolume() { return m_sound_volume; }

	// Enable/disable music
	static void enableMusic(bool enabled);
	static bool isMusicEnabled();

	// Enable/disable sound effects
	static void enableSound(bool enabled);
	static bool isSoundEnabled();

	// Stop music and sound effects
	static void stopAll();

private:
	static const int MAX_SOUNDS = 20;

	static sf::Sound    m_sounds[MAX_SOUNDS];
	static int          m_last_sound_played;
	static sf::Music    m_music;
	static std::string  m_music_name;
	static int          m_music_volume;
	static int          m_sound_volume;
	static bool         m_enable_music;
	static bool         m_enable_sound;
};

#endif // SOUNDSYSTEM_HPP
