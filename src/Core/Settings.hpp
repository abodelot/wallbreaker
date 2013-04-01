#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <cstdlib>

class Settings
{
public:
	static int    highscore;
	static bool   sound;
	static size_t screen_width;
	static size_t screen_height;

	static bool saveToFile(const char* filename);

	static bool loadFromFile(const char* filename);
};

#endif // SETTINGS_HPP
