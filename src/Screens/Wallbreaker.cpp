#include <iostream>

#include "Wallbreaker.hpp"
#include "Entities/Ball.hpp"
#include "Entities/LaserBeam.hpp"
#include "Entities/PowerUp.hpp"
#include "Core/Easing.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
#include "Core/HUD.hpp"
#include "Gui/Theme.hpp"


Wallbreaker::Wallbreaker():
	m_nb_active_bricks(0),
	m_particles(ParticleSystem::instance()),
	m_info_text(gui::Theme::getFont()),
	m_status(READY),
	m_player_lives(5),
	m_current_level(0)
{
	// Initialize render texture
	m_game_texture.create(m_width, m_height);

	// Sprites used for render textures
	m_game_sprite.setTexture(m_game_texture.getTexture());
	m_game_sprite.setPosition(GAME_BORDER_SIZE, GAME_BORDER_SIZE);

	m_hud_sprite.setTexture(HUD::getInstance().getTexture());
	m_hud_sprite.setPosition(0, m_height + GAME_BORDER_SIZE);
}


Wallbreaker::~Wallbreaker()
{
	if (m_level_file.is_open())
		m_level_file.close();
	clearEntities();
}


void Wallbreaker::onEvent(const sf::Event& event)
{
	switch (m_status)
	{
		case READY:
			if (event.type == sf::Event::MouseButtonPressed)
			{
				setStatus(PLAYING);
			}
			break;
		case PLAYING:
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					LaserBeam* beam = new LaserBeam();
					beam->setPosition(m_paddle.getPosition().x + (m_paddle.getWidth() - beam->getWidth()) / 2,
					           m_height - m_paddle.getHeight() - beam->getHeight());
					m_entities.push_back(beam);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					createBall();
				}
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
					case sf::Keyboard::F2:
						Game::getInstance().takeScreenshot();
						break;
					case sf::Keyboard::R:
						//todo: reload level
						std::cout << "relaod\n";
						break;
					case sf::Keyboard::N:
						std::cout << "next\n";
						break;
					default:
						break;
				}
			}
			break;
		case GAME_OVER:
			if (event.type == sf::Event::MouseButtonPressed)
			{
				Game::getInstance().previousScreen();
			}
			break;
	}
}


void Wallbreaker::onFocus()
{
	// Load first level
	m_current_level = 0;
	loadNextLevel();

	// Player paddle positioned at the bottom-center
	m_paddle.setPosition((m_width - m_paddle.getWidth()) / 2, m_height - m_paddle.getHeight());

	m_player_lives = 5;
	HUD::getInstance().setLiveCount(m_player_lives);
	setStatus(READY);

	HUD::getInstance().setScore(0);
	HUD::getInstance().setHighscore(0); // TODO
	Game::getInstance().getWindow().setMouseCursorVisible(false);
}


void Wallbreaker::update(float frametime)
{
	Easing::update(frametime);

	// Update player paddle and make sure it remains inside bounds
	m_paddle.onUpdate(frametime);
	if (m_paddle.getX() < 0)
		m_paddle.setX(0);
	else if (m_paddle.getX() + m_paddle.getWidth() > m_width)
		m_paddle.setX(m_width - m_paddle.getWidth());

	switch (m_status)
	{
		case READY:
		{
			Entity* ball = m_entities.front();
			// Keep the ball centered on the player pad
			ball->setPosition(m_paddle.getX() + (m_paddle.getWidth() - ball->getWidth()) / 2,
				              m_height - m_paddle.getHeight() - ball->getHeight());
			break;
		}
		case PLAYING:
			updateEntities(frametime);
			break;
		default:
			break;
	}

	m_particles.update(frametime);
}


void Wallbreaker::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw bricks
	m_game_texture.clear({0x16, 0x1e, 0x26});
	for (int i = 0; i < NB_BRICK_LINES; ++i)
	{
		for (int j = 0; j < NB_BRICK_COLS; ++j)
		{
			const Brick& brick = m_bricks[i][j];
			if (brick.getType() != Brick::NONE)
				m_game_texture.draw(brick, states);
		}
	}
	// Draw particles
	m_game_texture.draw(m_particles, states);

	// Draw entities
	m_game_texture.draw(m_paddle);
	for (EntityList::const_iterator it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		m_game_texture.draw(**it, states);
	}

	if (m_status != PLAYING)
	{
		sf::RectangleShape overlay({m_width, m_height});
		overlay.setFillColor({0, 0, 0, 128});
		m_game_texture.draw(overlay);
		m_game_texture.draw(m_info_text, states);
	}
	m_game_texture.display();

	// Draw sprites
	target.draw(m_hud_sprite);
	target.draw(m_game_sprite);
}


void Wallbreaker::updateEntities(float frametime)
{
	// For each entity
	for (EntityList::iterator it = m_entities.begin(); it != m_entities.end();)
	{
		Entity& entity = **it;
		if (entity.isAlive())
		{
			sf::Vector2f previous_entity_pos = entity.getPosition();
			entity.onUpdate(frametime);

			// Ensure entity doesn't go beyond the walls
			if (entity.getY() < 0)
			{
				entity.onCeilHit();
				entity.setY(0);
			}
			else if (entity.getPosition().x < 0)
			{
				entity.onWallHit();
				entity.setX(0);
			}
			else if (entity.getX() + entity.getWidth() > m_width)
			{
				entity.onWallHit();
				entity.setX(m_width - entity.getWidth());
			}
			// Kill entities if they go beyond the bottom line
			else if (entity.getY() + entity.getHeight() > m_height)
			{
				entity.kill();
			}
			// Check if entity collides with player pad
			else if (entity.collidesWith(m_paddle))
			{
				entity.onCollide(m_paddle);
				entity.setY(m_height - m_paddle.getHeight() - entity.getHeight());
			}
			// Check if entity is colliding with a brick
			else
			{
				// Get corners
				int left =   (int) entity.getX() / Brick::WIDTH;
				int top =    (int) entity.getY() / Brick::HEIGHT;
				int right =  (int) (entity.getX() + entity.getWidth() - 1)  / Brick::WIDTH;
				int bottom = (int) (entity.getY() + entity.getHeight() - 1) / Brick::HEIGHT;

				if (checkBrick(entity, top, left)
				 || checkBrick(entity, top, right)
				 || checkBrick(entity, bottom, left)
				 || checkBrick(entity, bottom, right))
				{
					entity.setPosition(previous_entity_pos);
					if (m_nb_active_bricks == 0)
					{
						if (loadNextLevel())
							setStatus(READY);
						else
							setStatus(GAME_OVER);
						break;
					}
				}
			}
			++it;
		}
		else
		{
			// Remove dead entity from entity list
			delete *it;
			it = m_entities.erase(it);
			if (Ball::getCount() == 0)
			{
				HUD::getInstance().setLiveCount(--m_player_lives);
				SoundSystem::playSound("life-lost.ogg");
				setStatus(m_player_lives == 0 ? GAME_OVER : READY);
				break;
			}
		}
	}
}


bool Wallbreaker::checkBrick(Entity& entity, int i, int j)
{
	Brick& brick = m_bricks[i][j];
	if (brick.isActive())
	{
		entity.onBrickHit(m_bricks[i][j]);
		if (!brick.isActive())
		{
			--m_nb_active_bricks;
			HUD::getInstance().setBrickCount(m_nb_active_bricks);
		}
		return true;
	}
	return false;
}

bool Wallbreaker::loadNextLevel()
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

	m_nb_active_bricks = 0;

	if (m_level_file.eof())
		return false;

	std::string line;
	for (int i = 0; i < NB_BRICK_LINES; ++i)
	{

		std::getline(m_level_file, line);
		if (m_level_file.eof())
		{
			break;
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
				++m_nb_active_bricks;
			// Reset brick position and rotation
			brick.setPosition(j * Brick::WIDTH, i * Brick::HEIGHT);
			brick.setRotation(0);
			brick.setScale(1, 1);
			printf("%c", line[j]);
		}
		puts("");
	}

	std::cout << "level " << ++m_current_level << " loaded (" << m_nb_active_bricks << " bricks)"<< std::endl;
	HUD::getInstance().setLevel(m_current_level);
	HUD::getInstance().setBrickCount(m_nb_active_bricks);
	SoundSystem::playSound("level-complete.ogg");
	Easing::stopAll();
	return true;
}


void Wallbreaker::setStatus(Status status)
{
	m_status = status;
	if (status == READY)
	{
		// Clear all and spawn a new ball
		clearEntities();
		createBall();
		m_info_text.setString("Ready?");
		m_info_text.setScale(2, 2);
		m_info_text.setPosition((m_width - m_info_text.getSize().x) / 2, (m_height - m_info_text.getSize().y) / 2);
	}
	else if (status == GAME_OVER)
	{
		m_info_text.setString(
			m_player_lives > 0 ?
			"Congratulations!\nGame complete!" :
			"GAME OVER.\nTry harder\nnext time!");
		m_info_text.setScale(2, 2);
		m_info_text.setPosition((m_width - m_info_text.getSize().x) / 2, (m_height - m_info_text.getSize().y) / 2);

		m_level_file.close();
	}
}


void Wallbreaker::clearEntities()
{
	for (EntityList::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		delete *it;
	}
	m_entities.clear();
}


void Wallbreaker::createBall()
{
	Ball* ball = new Ball();
	// Center ball on player pad
	float x = m_paddle.getPosition().x + (m_paddle.getWidth() - ball->getWidth()) / 2;
	float y = m_height - m_paddle.getHeight() - ball->getHeight();
	ball->setPosition(x, y);
	ball->launchParticles();
	m_entities.push_back(ball);
}
