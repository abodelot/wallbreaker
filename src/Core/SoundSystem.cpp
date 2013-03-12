#include <iostream>

#include "SoundSystem.hpp"
#include "Resources.hpp"

#define DEFAULT_MUSIC  "space.mod"
#define DEFAULT_VOLUME 40



sf::Sound        SoundSystem::m_sounds[MAX_SOUNDS];
int              SoundSystem::m_last_sound_played = 0;
sf::Music        SoundSystem::m_music;
std::string      SoundSystem::m_music_name   = DEFAULT_MUSIC;
int              SoundSystem::m_music_volume = DEFAULT_VOLUME;
int              SoundSystem::m_sound_volume = DEFAULT_VOLUME;
bool             SoundSystem::m_enable_music = true;
bool             SoundSystem::m_enable_sound = true;

template <class T>
static inline void clamp(T& value, T min, T max)
{
	if (value > max)
		value = max;
	else if (value < min)
		value = min;
}


void SoundSystem::setMusic(const std::string& music_name)
{
	if (m_music_name != music_name)
	{
		stopMusic();
		m_music.openFromFile(music_name);
		m_music.setVolume(m_music_volume);
		m_music_name = music_name;
	}
}


const std::string& SoundSystem::getMusic()
{
	return m_music_name;
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


void SoundSystem::playSound(const std::string& sound_name, float pitch)
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
		sound.setBuffer(Resources::getSoundBuffer(sound_name));
		sound.setPitch(pitch);
		sound.play();
		++m_last_sound_played;
	}
}


void SoundSystem::setMusicVolume(int volume)
{
	clamp(volume, 0, 100);
	m_music_volume = volume;
	if (!m_music_name.empty())
	{
		m_music.setVolume(volume);
	}
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

