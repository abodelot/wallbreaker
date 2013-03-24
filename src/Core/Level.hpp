#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <fstream>
#include "Config.hpp"
#include "Entities/Brick.hpp"

class Level
{
public:
	Level();

	void reset();

	bool loadLevel(size_t level_index);

	bool loadPreviousLevel();

	bool loadNextLevel();

	int getCurrentLevel() const;

	Brick& getBrick(int i, int j);

	size_t getLevelCount() const;

private:

	bool load();

	Brick         m_bricks[NB_BRICK_LINES][NB_BRICK_COLS];
	std::ifstream m_level_file;
	int           m_current_level;
};

#endif // LEVEL_HPP
