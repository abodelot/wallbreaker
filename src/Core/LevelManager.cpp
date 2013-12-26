#include <iostream>
#include "LevelManager.hpp"

/**
 * Number of bytes per level in the levels file
 * NB_BRICK_COLS + 1 because additional '\n' character for each line
 */
#define LEVEL_SIZE() ((NB_BRICK_COLS + 1) * NB_BRICK_LINES)


LevelManager& LevelManager::getInstance()
{
	static LevelManager self;
	return self;
}


LevelManager::LevelManager():
	m_current_level(0),
	m_level_count(0)
{
	// Initialize bricks position
	for (int i = 0; i < NB_BRICK_LINES; ++i)
		for (int j = 0; j < NB_BRICK_COLS; ++j)
			m_bricks[i][j].setPosition(j * Brick::WIDTH, i * Brick::HEIGHT);
}


LevelManager::~LevelManager()
{
	if (m_level_file.is_open())
		m_level_file.close();
}


bool LevelManager::openFromFile(const std::string& filename)
{
	m_level_file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
	if (m_level_file)
	{
		// Number of levels = file size / size of one level
		m_level_file.seekg(0, std::ifstream::end);
		m_level_count = m_level_file.tellg() / LEVEL_SIZE();

		// Restore cursor position
		m_level_file.seekg(0);
		return true;
	}
	std::cerr << "error while opening level file " << filename << std::endl;
	return false;
}


int LevelManager::loadAt(size_t index)
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


int LevelManager::loadPrevious()
{
	return loadAt(m_current_level - 1);
}


int LevelManager::reload()
{
	return loadAt(m_current_level);
}


int LevelManager::loadNext()
{
	return load();
}


void LevelManager::save()
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


size_t LevelManager::append()
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


size_t LevelManager::getCurrentLevel() const
{
	return m_current_level;
}


Brick& LevelManager::getBrick(int i, int j)
{
	return m_bricks[i][j];
}


size_t LevelManager::getLevelCount() const
{
	return m_level_count;
}


size_t LevelManager::getBrickCount() const
{
	return m_brick_count;
}


int LevelManager::load()
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
	printf("level %d/%d loaded (contains %u bricks)\n", m_current_level, m_level_count, m_brick_count);
#endif

	return m_brick_count;
}


void LevelManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw background gradient
	sf::Vertex background[4];
	background[1].position.x = target.getSize().x;
	background[2].position.x = target.getSize().x;
	background[2].position.y = target.getSize().y;
	background[3].position.y = target.getSize().y;

	background[0].color = {0x1a, 0x35, 0x49};
	background[1].color = {0x1a, 0x35, 0x49};
	background[2].color = {0x2c, 0x80, 0xb8};
	background[3].color = {0x2c, 0x80, 0xb8};

	target.draw(background, 4, sf::Quads, states);

	// Cast borders shadow
	sf::Vertex shadow[4];
	for (int i = 0; i < 4; ++i)
		shadow[i].color = sf::Color(0, 0, 0, 96);

	// Vertical top border
	shadow[1].position.x = BORDER_SIZE / 2;
	shadow[2].position.x = BORDER_SIZE / 2;
	shadow[2].position.y = target.getSize().y;
	shadow[3].position.y = target.getSize().y;
	target.draw(shadow, 4, sf::Quads, states);

	// Horizontal left border
	shadow[0].position.x = BORDER_SIZE / 2;
	shadow[1].position.x = target.getSize().x;
	shadow[2].position.x = target.getSize().x;
	shadow[2].position.y = BORDER_SIZE / 2;
	shadow[3].position.x = shadow[0].position.x;
	shadow[3].position.y = shadow[2].position.y;
	target.draw(shadow, 4, sf::Quads, states);

	// Draw bricks
	for (int i = 0; i < NB_BRICK_LINES; ++i)
	{
		for (int j = 0; j < NB_BRICK_COLS; ++j)
		{
			const Brick& brick = m_bricks[i][j];
			if (brick.isActive())
			{
				// Cast brick shadow
				for (int i = 0; i < 4; ++i)
					shadow[i].position = brick.getPosition() + sf::Vector2f(3, 3);

				shadow[1].position.x += Brick::WIDTH;
				shadow[2].position.x += Brick::WIDTH;
				shadow[2].position.y += Brick::HEIGHT;
				shadow[3].position.y += Brick::HEIGHT;
				target.draw(shadow, 4, sf::Quads, states);
			}

			if (brick.getType() != Brick::NONE)
				target.draw(brick, states);
		}
	}
}
