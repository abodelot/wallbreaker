#include <iostream>
#include "Level.hpp"


Level::Level():
	m_level_file(LEVEL_FILE),
	m_current_level(0)
{
	if (m_level_file)
	{
		// Initialize bricks position
		for (int i = 0; i < NB_BRICK_LINES; ++i)
			for (int j = 0; j < NB_BRICK_COLS; ++j)
				m_bricks[i][j].setPosition(j * Brick::WIDTH, i * Brick::HEIGHT);

		load();
	}
	else
	{
		std::cerr << "Couldn't open level file:" << LEVEL_FILE << std::endl;
	}


}


void Level::reset()
{
}


bool Level::loadLevel(size_t level_index)
{
}


bool Level::loadPreviousLevel()
{
}


bool Level::loadNextLevel()
{
}


int Level::getCurrentLevel() const
{
	return m_current_level;
}


Brick& Level::getBrick(int i, int j)
{
	return m_bricks[i][j];
}


size_t Level::getLevelCount() const
{
}


bool Level::load()
{
	if (!m_level_file.is_open())
	{
		std::string filename = Game::getInstance().getCurrentDir() + LEVEL_FILE;
		m_level_file.open(filename.c_str());
		if (!m_level_file)
		{
			std::cerr << "error while opening level file " << filename << std::endl;
			return false;
		}
	}

	int nb_active_bricks = 0;

	if (m_level_file.eof())
		return false;

	std::string line;
	for (int i = 0; i < NB_BRICK_LINES; ++i)
	{
		std::getline(m_level_file, line);
		if (m_level_file.eof())
		{
			return false;
		}
		int length = line.size();
		printf("%02d. ", i);
		for (int j = 0; j < NB_BRICK_COLS && j < length; ++j)
		{
			Brick& brick = m_bricks[i][j];
			// Set brick type
			int type = line[j];
			brick.setType(type);
			if (type != Brick::NONE && type != Brick::UNBREAKABLE)
				++nb_active_bricks;
			// Reset brick position and rotation
			brick.setPosition(j * Brick::WIDTH, i * Brick::HEIGHT);
			brick.setRotation(0);
			brick.setScale(1, 1);
			printf("%c", line[j]);
		}
		puts("");
	}

	std::cout << "level " << ++m_current_level << " loaded (" << nb_active_bricks << " bricks)"<< std::endl;
	return true;
}
