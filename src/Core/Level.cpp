#include <iostream>
#include "Level.hpp"

/**
 * Number of bytes per level in the levels file
 * NB_BRICK_COLS + 1 because additional '\n' character for each line
 */
#define LEVEL_SIZE() ((NB_BRICK_COLS + 1) * NB_BRICK_LINES)


Level::Level():
	m_current_level(0),
	m_level_count(0)
{
	// Initialize bricks position
	for (int i = 0; i < NB_BRICK_LINES; ++i)
		for (int j = 0; j < NB_BRICK_COLS; ++j)
			m_bricks[i][j].setPosition(j * Brick::WIDTH, i * Brick::HEIGHT);


	// Open the file containing the levels
	std::string filename = Game::getInstance().getCurrentDir() + LEVEL_FILE;
	m_level_file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);
	if (!m_level_file)
	{
		std::cerr << "error while opening level file " << filename << std::endl;
	}
	else
	{
		// Number of levels = file size / size of one level
		m_level_file.seekg(0, std::ifstream::end);
		m_level_count = m_level_file.tellg() / LEVEL_SIZE();
		std::cout << "level count:" << m_level_count << std::endl;

		// Load first level
		m_level_file.seekg(0);
		load();
	}
}


Level::~Level()
{
	if (m_level_file.is_open())
		m_level_file.close();
}


bool Level::loadAt(size_t index)
{
	if (index > 0 && index <= m_level_count)
	{
		// -1 because stream cursor needs to be BEFORE the level
		m_current_level = index - 1;
		m_level_file.seekg(LEVEL_SIZE() * m_current_level);
		return load();
	}
	return false;
}


bool Level::loadPrevious()
{
	return loadAt(m_current_level - 1);
}


bool Level::reload()
{
	return loadAt(m_current_level);
}


bool Level::loadNext()
{
	return load();
}


void Level::save()
{
	// Set stream cursor before current level
	m_level_file.seekp(LEVEL_SIZE() * (m_current_level - 1));
	for (int i = 0; i < NB_BRICK_LINES; ++i)
	{
		for (int j = 0; j < NB_BRICK_COLS; ++j)
		{
			Brick& brick = m_bricks[i][j];
			m_level_file.put(brick.getType());
		}
		m_level_file.put('\n');
	}
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
	return m_level_count;
}


bool Level::load()
{


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
