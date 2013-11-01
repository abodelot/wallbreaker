#include "Wallbreaker.hpp"
#include "OptionsMenu.hpp"

#include "Entities/LaserBeam.hpp"
#include "Entities/PowerUp.hpp"
#include "Core/Effect.hpp"
#include "Core/Resources.hpp"
#include "Core/Settings.hpp"
#include "Core/SoundSystem.hpp"
#include "Gui/Theme.hpp"
#include "Utils/Math.hpp"

#define MAX_PLAYER_LIVES 5


Wallbreaker::Wallbreaker():
	m_width(NB_BRICK_COLS * Brick::WIDTH),
	m_height(NB_BRICK_LINES * Brick::HEIGHT),
	m_remaining_bricks(0),
	m_particles(ParticleSystem::instance()),
	m_info_text(gui::Theme::getFont()),
	m_score(0),
	m_status(READY),
	m_player_lives(MAX_PLAYER_LIVES),
	m_pause_menu(Game::getInstance().getWindow()),
	m_game_over_menu(Game::getInstance().getWindow())
{
	// Initialize render texture
	m_level_texture.create(m_width, m_height);

	// Sprites used for render textures
	m_level_sprite.setTexture(m_level_texture.getTexture());
	m_level_sprite.setOrigin(m_width / 2, m_height / 2);
	m_level_sprite.setPosition(m_width / 2 + GAME_BORDER_SIZE, m_height / 2 + GAME_BORDER_SIZE);

	m_hud_sprite.setTexture(m_hud.getTexture());
	m_hud_sprite.setPosition(m_width + GAME_BORDER_SIZE * 2, 0);

	m_borders_sprite.setTexture(Resources::getTexture("borders.png"));

	// Player paddle positioned at the bottom-center
	m_paddle.setPosition((m_width - m_paddle.getWidth()) / 2, m_height - m_paddle.getHeight());
	m_paddle.setParent(this);

	m_remaining_bricks = m_level.getBrickCount();

	// Initialize HUD
	m_hud.setLiveCount(m_player_lives);
	m_hud.setBrickCount(m_remaining_bricks);
	m_hud.setLevel(m_level.getCurrentLevel());
	m_hud.setScore(m_score);
	m_hud.setHighscore(Settings::highscore);
	setStatus(READY);

	Effect::stopAll();
	m_level_sprite.setScale(0, 0);
	Effect::zoom(m_level_sprite, 1);

	// Pause menu
	m_pause_menu.setPosition(GAME_BORDER_SIZE + (m_width - gui::Theme::WIDGET_WIDTH) / 2, 120);
	m_pause_menu.addButton("Resume",    1);
	m_pause_menu.addButton("Options",   2);
	m_pause_menu.addButton("Main menu", 3);

	// Game over menu
	m_game_over_menu.setPosition(m_pause_menu.getPosition());
	m_game_over_menu.addButton("Continue",  1);
	m_game_over_menu.addButton("Main menu", 2);
}


Wallbreaker::~Wallbreaker()
{
	clearEntities();
	m_particles.clear();
}


void Wallbreaker::onEvent(const sf::Event& event)
{
	switch (m_status)
	{
		case PLAYING:
			switch (event.type)
			{
				case sf::Event::KeyPressed:
					switch (event.key.code)
					{
						case sf::Keyboard::F2:
							Game::getInstance().takeScreenshot();
							break;
						case sf::Keyboard::Escape:
							setStatus(PAUSED);
							break;
						case sf::Keyboard::Space:
							if (m_paddle.isSticky())
								applyOnEachBall(&Ball::unstick);
							else if (m_paddle.hasLaser())
								addEntity(m_paddle.shoot());
							break;
#ifdef WALLBREAKER_DEBUG
						case sf::Keyboard::R: // Reload level
							m_remaining_bricks = m_level.reload();
							m_hud.setBrickCount(m_remaining_bricks);
							break;
						case sf::Keyboard::N: // Go to next level
							loadNextLevel();
							break;
						case sf::Keyboard::G: // Simulate LARGE_PADDLE
							m_paddle.grow();
							break;
						case sf::Keyboard::S: // Simulate SMALL_PADDLE
							m_paddle.shrink();
							break;
						case sf::Keyboard::Y: // Simulate STICKY_PADDLE
							m_paddle.activeSticky();
							break;
						case sf::Keyboard::L: // Simulate LASER_PADDLE
							m_paddle.activeLaser();
							break;
						case sf::Keyboard::T: // Simulate TRIPLE_BALL
							createBall();
							createBall();
							break;
						case sf::Keyboard::P: // Simulate POWER_BALL
							applyOnEachBall(&Ball::enablePowerBall);
							break;
						case sf::Keyboard::E: // Simulate SPEED_RESET
							applyOnEachBall(&Ball::resetSpeed);
							break;
#endif
						default:
							break;
					}
					break;

				case sf::Event::MouseButtonPressed:
					if (m_paddle.isSticky())
						applyOnEachBall(&Ball::unstick);
					else if (m_paddle.hasLaser())
						addEntity(m_paddle.shoot());
					break;

				case sf::Event::LostFocus:
					setStatus(PAUSED);
					break;

				default:
					break;
			}
			break;

		case PAUSED:
			switch (m_pause_menu.onEvent(event))
			{
				case 1: // Resume
					setStatus(PLAYING);
					break;
				case 2: // Go to options menu
					Game::getInstance().nextScreen(new OptionsMenu);
					break;
				case 3: // Back to main menu
					Game::getInstance().previousScreen();
					break;
				default:
					// Resume
					if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
						setStatus(PLAYING);
					break;
			}
			break;

		case READY:
			if (event.type == sf::Event::MouseButtonPressed)
				setStatus(PLAYING);
			break;

		case GAME_OVER:
			switch (m_game_over_menu.onEvent(event))
			{
				case 1: // Continue
					Effect::stopAll();

					// Reset score
					m_score = 0;
					m_hud.setScore(0);

					// Reset lives
					m_player_lives = MAX_PLAYER_LIVES;
					m_hud.setLiveCount(MAX_PLAYER_LIVES);

					// Reload current level
					m_remaining_bricks = m_level.reload();
					m_hud.setBrickCount(m_remaining_bricks);
					setStatus(READY);
					break;
				case 2: // Back to main menu
					Game::getInstance().previousScreen();
					break;
			}
			break;
	}
}


void Wallbreaker::update(float frametime)
{
	if (m_status == PLAYING || m_status == READY)
	{
		Effect::update(frametime);

		// Update player paddle and make sure it remains inside bounds
		m_paddle.onUpdate(frametime);
		if (m_paddle.getX() < 0)
			m_paddle.setX(0);
		else if (m_paddle.getX() + m_paddle.getWidth() > m_width)
			m_paddle.setX(m_width - m_paddle.getWidth());

		if (m_status == PLAYING)
		{
			updateEntities(frametime);
		}
		else
		{
			Entity* ball = m_entities.front();
			// Keep the ball centered on the player pad
			ball->setPosition(m_paddle.getX() + (m_paddle.getWidth() - ball->getWidth()) / 2,
				              m_height - m_paddle.getHeight() - ball->getHeight());
		}
		m_particles.update(frametime);
	}
	updateTexture();
}


void Wallbreaker::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_borders_sprite, states);
	target.draw(m_level_sprite, states);
	target.draw(m_hud_sprite, states);

	if (m_status != PLAYING)
	{
		sf::RectangleShape overlay({APP_WIDTH, APP_HEIGHT});
		overlay.setFillColor({0, 0, 0, 128});
		target.draw(overlay);
		target.draw(m_info_text);
		if (m_status == PAUSED)
		{
			m_pause_menu.draw();
		}
		else if (m_status == GAME_OVER)
		{
			m_game_over_menu.draw();
		}
	}
}


void Wallbreaker::updateTexture()
{
	// Draw bricks
	m_level_texture.draw(m_level);

	// Draw particles
	m_level_texture.draw(m_particles);

	// Draw entities
	m_level_texture.draw(m_paddle);
	for (EntityList::const_iterator it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		m_level_texture.draw(**it);
	}

	m_level_texture.display();
}


void Wallbreaker::addPlayerLife()
{
	if (m_player_lives < MAX_PLAYER_LIVES)
		m_hud.setLiveCount(++m_player_lives);
}


void Wallbreaker::applyOnEachBall(Ball::ActionPointer action)
{
	for (EntityList::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		Ball* ball = (**it).toBall();
		if (ball != NULL)
		{
			(ball->*action)();
		}
	}
}


void Wallbreaker::updateEntities(float frametime)
{
	// For each entity
	for (EntityList::iterator it = m_entities.begin(); it != m_entities.end();)
	{
		Entity& entity = **it;
		if (entity.isAlive())
		{
			sf::Vector2f old_pos = entity.getPosition();
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
			else if (entity.getY() > m_height)
			{
				entity.kill();
			}
			// Check if entity collides with paddle
			else if ((entity.getY() + entity.getHeight() > m_paddle.getY())
			      && (entity.getY() < m_paddle.getY())
			      && (entity.getX() + entity.getWidth() > m_paddle.getX())
			      && (entity.getX() < m_paddle.getX() + m_paddle.getWidth()))
			{
				entity.setY(m_height - m_paddle.getHeight() - entity.getHeight());
				entity.onPaddleHit(m_paddle);
			}
			// Check if entity is colliding with a brick
			else
			{
				// Get corners
				int left =   (int) entity.getX() / Brick::WIDTH;
				int top =    (int) entity.getY() / Brick::HEIGHT;
				int right =  (int) (entity.getX() + entity.getWidth())  / Brick::WIDTH;
				int bottom = (int) (entity.getY() + entity.getHeight()) / Brick::HEIGHT;

				if (checkBrick(entity, top, left, old_pos)
				 || checkBrick(entity, top, right, old_pos)
				 || checkBrick(entity, bottom, left, old_pos)
				 || checkBrick(entity, bottom, right, old_pos))
				{
					if (m_remaining_bricks == 0)
					{
						if (m_level.getCurrentLevel() == m_level.getLevelCount())
						{
							setStatus(GAME_OVER);
						}
						else
						{
							loadNextLevel();
							setStatus(READY);
						}
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
				m_hud.setLiveCount(--m_player_lives);
				SoundSystem::playSound("life-lost.ogg");
				setStatus(m_player_lives == 0 ? GAME_OVER : READY);
				break;
			}
		}
	}
}


bool Wallbreaker::checkBrick(Entity& entity, int i, int j, const sf::Vector2f& old_pos)
{
	if (i >= 0 && i < NB_BRICK_LINES && j >= 0 && j < NB_BRICK_COLS)
	{
		Brick& brick = m_level.getBrick(i, j);
		if (brick.isActive())
		{
			entity.onBrickHit(brick, old_pos);

			if (!brick.isActive())
			{
				// Randomly create a new power-up
				if (math::rand(0, 9) == 0)
				{
					PowerUp* powerup = PowerUp::createRandom();
					powerup->setPosition(brick.getPosition());
					addEntity(powerup);
				}
				// Update remaining bricks counter
				--m_remaining_bricks;
				m_hud.setBrickCount(m_remaining_bricks);
				// Upgrade score
				++m_score;
				m_hud.setScore(m_score);
				if (m_score > Settings::highscore)
				{
					Settings::highscore = m_score;
					m_hud.setHighscore(Settings::highscore);
				}

			}
			return true;
		}
	}
	return false;
}


void Wallbreaker::loadNextLevel()
{
	Effect::stopAll();
	m_remaining_bricks = m_level.loadNext();
	m_hud.setLevel(m_level.getCurrentLevel());
	m_hud.setBrickCount(m_remaining_bricks);
	SoundSystem::playSound("level-complete.ogg");

	m_level_sprite.setScale(0, 0);
	Effect::zoom(m_level_sprite, 1);
}


void Wallbreaker::setStatus(Status status)
{
	m_status = status;
	switch (status)
	{
	case READY:
		// Remove power-ups applied to the paddle
		m_paddle.reset();

		// Clear all and spawn a new ball
		clearEntities();
		createBall();
		m_info_text.setString("Ready?");
		Game::getInstance().getWindow().setMouseCursorVisible(false);
		break;

	case GAME_OVER:
		m_info_text.setString(m_player_lives > 0 ? "Game complete!" : "Game Over");
		Game::getInstance().getWindow().setMouseCursorVisible(true);
		break;

	case PAUSED:
		m_info_text.setString("Paused");
		Game::getInstance().getWindow().setMouseCursorVisible(true);
		break;

	case PLAYING:
		Game::getInstance().getWindow().setMouseCursorVisible(false);
		break;
	}

	m_info_text.setScale(2, 2);
	m_info_text.setPosition(GAME_BORDER_SIZE + (m_width - m_info_text.getSize().x) / 2, 80);
}


void Wallbreaker::createBall()
{
	Ball* ball = new Ball();
	// Center ball on player pad
	float x = m_paddle.getPosition().x + (m_paddle.getWidth() - ball->getWidth()) / 2;
	float y = m_height - m_paddle.getHeight() - ball->getHeight();
	ball->setPosition(x, y);
	ball->launchParticles();
	addEntity(ball);
}


void Wallbreaker::addEntity(Entity* entity)
{
	entity->setParent(this);
	m_entities.push_back(entity);
}


void Wallbreaker::clearEntities()
{
	for (EntityList::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		delete *it;
	}
	m_entities.clear();
}


