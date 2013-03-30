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
#ifdef WALLBREAKER_DEBUG
		printf("%s contains %u levels\n", LEVEL_FILE, m_level_count);
#endif
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


int Level::loadAt(size_t index)
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


int Level::loadPrevious()
{
	return loadAt(m_current_level - 1);
}


int Level::reload()
{
	return loadAt(m_current_level);
}


int Level::loadNext()
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


size_t Level::append()
{
	// Create an empty level
	for (int i = 0; i < NB_BRICK_LINES; ++i)
		for (int j = 0; j < NB_BRICK_COLS; ++j)
			m_bricks[i][j].setType(Brick::NONE);

	// Save it at the end of the file
	m_current_level = ++m_level_count;
	save();
	return m_current_level;
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


size_t Level::getBrickCount() const
{
	return m_brick_count;
}


int Level::load()
{
	m_brick_count = 0;

	if (m_level_file.eof())
		return 0;

	std::string line;
	for (int i = 0; i < NB_BRICK_LINES; ++i)
	{
		std::getline(m_level_file, line);
		if (m_level_file.eof())
		{
			return 0;
		}
		int length = line.size();
#ifdef WALLBREAKER_DEBUG
		printf("%2d  ", i + 1);
#endif
		for (int j = 0; j < NB_BRICK_COLS && j < length; ++j)
		{
			Brick& brick = m_bricks[i][j];
			// Set brick type
			int type = line[j];
			brick.setType(type);
			if (type != Brick::NONE && type != Brick::UNBREAKABLE)
				++m_brick_count;
			// Reset brick position and rotation
			brick.setPosition(j * Brick::WIDTH, i * Brick::HEIGHT);
			brick.setRotation(0);
			brick.setScale(1, 1);
#ifdef WALLBREAKER_DEBUG
			putchar(line[j]);
#endif
		}
#ifdef WALLBREAKER_DEBUG
		putchar('\n');
#endif
	}
	++m_current_level;
#ifdef WALLBREAKER_DEBUG
	printf("level %d loaded (contains %u bricks)\n", m_current_level, m_brick_count);
#endif

	return m_brick_count;
}


void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.clear({0x16, 0x1e, 0x26});
	for (int i = 0; i < NB_BRICK_LINES; ++i)
	{
		for (int j = 0; j < NB_BRICK_COLS; ++j)
		{
			const Brick& brick = m_bricks[i][j];
			if (brick.getType() != Brick::NONE)
				target.draw(brick, states);
		}
	}
}
