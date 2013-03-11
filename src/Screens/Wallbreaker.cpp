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

#define POWER_UP_RATIO 70
#define COMBO_DELAY 0.5

#define BORDER_SIZE 10 // frame around the playground


Wallbreaker::Wallbreaker():
	m_nb_active_bricks(0),
	m_status(READY),
	m_player_lives(5),
	m_current_level(0),
	m_particles(ParticleSystem::instance()),
	m_info_text(gui::Theme::getFont())
{
	sf::Texture& texture = Resources::getTexture("background-blue.gif");
	texture.setRepeated(true);
	m_background.setTexture(texture);
	m_background.setTextureRect(sf::IntRect(0, 0, APP_WIDTH, APP_HEIGHT));

	// Initialize render textures
	m_game_texture.create(m_width, m_height);

	m_hud.setTexture(HUD::getInstance().getTexture());
	m_hud.setPosition(0, m_height + BORDER_SIZE);
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
					beam->setPosition(m_player_pad.getPosition().x + (m_player_pad.getWidth() - beam->getWidth()) / 2,
					           m_height - m_player_pad.getHeight() - beam->getHeight());
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

	// Player pad positioned at the bottom-center
	m_player_pad.setPosition((m_width - m_player_pad.getWidth()) / 2,
							 m_height - m_player_pad.getHeight());

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

	// Update player pad and make sure it remains inside bounds
	m_player_pad.onUpdate(frametime);
	if (m_player_pad.getPosition().x < 0)
		m_player_pad.setX(0);
	else if (m_player_pad.getPosition().x + m_player_pad.getWidth() > m_width)
		m_player_pad.setX(m_width - m_player_pad.getWidth());

	switch (m_status)
	{
		case READY:
		{
			Entity* ball = m_entities.front();
			// Keep the ball on the player pad
			ball->setPosition(m_player_pad.getPosition().x + (m_player_pad.getWidth() - ball->getWidth()) / 2,
				              m_height - m_player_pad.getHeight() - ball->getHeight());
		}

			break;
		case PLAYING:
		{
			// Update entities
			for (EntityList::iterator it = m_entities.begin(); it != m_entities.end();)
			{
				Entity& entity = **it;
				if (entity.isAlive())
				{
					sf::Vector2f previous_pos = entity.getPosition();
					entity.onUpdate(frametime);


					// Check if entity is still inside the bounds
					if (entity.getPosition().y < 0)
					{
						entity.onCeilHit();
						entity.setY(0);
					}
					else if (entity.getPosition().x < 0)
					{
						entity.onWallHit();
						entity.setX(0);
					}
					else if (entity.getPosition().x + entity.getWidth() > m_width)
					{
						entity.onWallHit();
						entity.setX(m_width - entity.getWidth());
					}
					else if (entity.getPosition().y + entity.getHeight() > m_height)
					{
						entity.kill();
					}
					// Check if ball collides with player pad
					else if (entity.collidesWith(m_player_pad))
					{
						entity.onCollide(m_player_pad);
						entity.setY(m_height - m_player_pad.getHeight() - entity.getHeight());
					}
					// Check if ball collides with brick
					else if (getCollidingBrick(entity) != NULL)
					{
						entity.setPosition(previous_pos);
						if (m_nb_active_bricks == 0)
						{
							if (loadNextLevel())
							{
								setStatus(READY);
							}
							else
							{
								setStatus(GAME_OVER);
							}
							break;
						}
					}
					++it;
				}
				else
				{
					delete *it;
					it = m_entities.erase(it);
					if (Ball::getCount() == 0)
					{
						HUD::getInstance().setLiveCount(--m_player_lives);
						SoundSystem::playSound("ball-lost.ogg");
						setStatus(m_player_lives == 0 ? GAME_OVER : READY);
						break;
					}
				}
			}
		}

		break;
	}

	m_particles.update(frametime);
}


void Wallbreaker::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//target.draw(m_background, states);

	// Draw bricks
	m_game_texture.clear({0x16, 0x1e, 0x26});
	for (int i = 0; i < m_brick_lines; ++i)
	{
		for (int j = 0; j < m_brick_cols; ++j)
		{
			const Brick& brick = m_bricks[i][j];
			if (brick.getType() != BRICK_NONE)
				m_game_texture.draw(brick, states);
		}
	}
	// Draw particles
	m_game_texture.draw(m_particles, states);

	// Draw entities
	m_game_texture.draw(m_player_pad);
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
	sf::Sprite s;
	s.setTexture(m_game_texture.getTexture(), true);
	s.setPosition(10, 10);
	target.draw(s);

	// Draw HUD
	target.draw(m_hud);
}


bool Wallbreaker::loadNextLevel()
{

	if (!m_level_file.is_open())
	{
		std::string filename = Game::getInstance().getCurrentDir() + "resources/levels/levels.txt";
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
	std::getline(m_level_file, line);
	if (line.empty())
		return false;

	sf::Texture& texture = Resources::getTexture(line);
	texture.setRepeated(true);
	m_background.setTexture(texture);
	for (int i = 0; i < m_brick_lines; ++i)
	{
		std::getline(m_level_file, line);
		if (m_level_file.eof())
		{
			break;
		}
		int length = line.size();
		printf("%02d. ", i);
		for (int j = 0; j < m_brick_cols && j < length; ++j)
		{
			Brick& brick = m_bricks[i][j];
			// Set brick type
			int type = line[j];
			brick.setType(type);
			if (type != BRICK_NONE && type != BRICK_UNBREAKABLE)
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


Brick* Wallbreaker::getCollidingBrick(Entity& entity)
{
	sf::IntRect rect = entity.getSurface();
	sf::IntRect brick_rect;

	// on calcule les indices de chaque coin
	int left = (int) rect.left / Brick::WIDTH;
	int top = (int) rect.top / Brick::HEIGHT;
	int right = (int) (rect.left + rect.width - 1) / Brick::WIDTH;
	int bottom = (int) (rect.top + rect.height - 1) / Brick::HEIGHT;

	//printf("cds at top %d left %d bottom %d right %d\n", top, left, bottom, right);

	Brick* collision[4] = {NULL, NULL, NULL, NULL};
	if (m_bricks[top][left].isActive())     collision[0] = &m_bricks[top][left];
	if (m_bricks[top][right].isActive())    collision[1] = &m_bricks[top][right];
	if (m_bricks[bottom][left].isActive())  collision[2] = &m_bricks[bottom][left];
	if (m_bricks[bottom][right].isActive()) collision[3] = &m_bricks[bottom][right];

	// search for larger surface
	Brick* brick = NULL;
	float max_surface = 0;
	bool is_vertical = false;
	for (int i = 0; i < 4; ++i)
	{
		if (collision[i] != NULL)
		{
			brick_rect.top = collision[i]->getPosition().y;
			brick_rect.left = collision[i]->getPosition().x;
			brick_rect.width = Brick::WIDTH;
			brick_rect.height = Brick::HEIGHT;
			sf::IntRect overlapp;
			rect.intersects(brick_rect, overlapp);

			float surface = overlapp.width * overlapp.height;
			if (surface > max_surface)
			{
				is_vertical = overlapp.height > overlapp.width;
				max_surface = surface;
				brick = collision[i];
			}
		}
	}

	// if collision detected
	if (brick != NULL)
	{
		is_vertical ? entity.onWallHit() : entity.onCeilHit();
		//brick->takeDamage();
		entity.onBrickHit(*brick);

		if (!brick->isActive())
		{
			--m_nb_active_bricks;
			HUD::getInstance().setBrickCount(m_nb_active_bricks);

			if (math::rand(0, 100) < POWER_UP_RATIO)
			{
				/*PowerUp* power_up = new PowerUp(PowerUp::EXTRA_BALL);
				power_up->setPosition(brick->getPosition().x, brick->getPosition().y + brick->getHeight());
				m_entities.push_back(power_up);*/
			}


		}

		return brick;
	}
	return NULL;
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
	float x = m_player_pad.getPosition().x + (m_player_pad.getWidth() - ball->getWidth()) / 2;
	float y = m_height - m_player_pad.getHeight() - ball->getHeight();
	ball->setPosition(x, y);
	ball->launchParticles();
	m_entities.push_back(ball);
}
