#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <fstream>
#include "Config.hpp"
#include "Entities/Brick.hpp"

class Level
{
public:
	Level();

	bool loadAt(size_t index);

	bool reload();

	bool loadPrevious();

	bool loadNext();

	/**
	 * Save current level to file
	 */
	void save();

	int getCurrentLevel() const;

	Brick& getBrick(int i, int j);

	size_t getLevelCount() const;

private:
	/**
	 * Load level at the current stream position
	 */
	bool load();

	Brick         m_bricks[NB_BRICK_LINES][NB_BRICK_COLS];
	std::ifstream m_level_file;
	int           m_current_level;
	int           m_level_count;
};

#endif // LEVEL_HPP
