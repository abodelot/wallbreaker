#include "Wallbreaker.hpp"
#include "OptionsMenu.hpp"

#include "Entities/Ball.hpp"
#include "Entities/LaserBeam.hpp"
#include "Entities/PowerUp.hpp"
#include "Core/Easing.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
#include "Core/HUD.hpp"
#include "Gui/Theme.hpp"


Wallbreaker::Wallbreaker():
	m_remaining_bricks(0),
	m_particles(ParticleSystem::instance()),
	m_info_text(gui::Theme::getFont()),
	m_status(READY),
	m_player_lives(5),
	m_menu(Game::getInstance().getWindow())
{
	// Initialize render texture
	m_level_texture.create(m_width, m_height);

	// Sprites used for render textures
	m_level_sprite.setTexture(m_level_texture.getTexture());
	m_level_sprite.setPosition(GAME_BORDER_SIZE, GAME_BORDER_SIZE);

	m_hud_sprite.setTexture(HUD::getInstance().getTexture());
	m_hud_sprite.setPosition(0, m_height + GAME_BORDER_SIZE);

	// Pause menu
	m_menu.setPosition(80, 100);
	m_menu.addButton("Resume", 1);
	m_menu.addButton("Options", 2);
	m_menu.addButton("Main menu", 3);

	// Player paddle positioned at the bottom-center
	m_paddle.setPosition((m_width - m_paddle.getWidth()) / 2, m_height - m_paddle.getHeight());

	m_player_lives = 5;
	m_remaining_bricks = m_level.getBrickCount();

	// Initialize HUD
	HUD::getInstance().setLiveCount(m_player_lives);
	HUD::getInstance().setBrickCount(m_remaining_bricks);
	HUD::getInstance().setLevel(m_level.getCurrentLevel());
	HUD::getInstance().setScore(0);
	HUD::getInstance().setHighscore(0);
	setStatus(READY);
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
						case sf::Keyboard::R:
							m_remaining_bricks = m_level.reload();
							HUD::getInstance().setBrickCount(m_remaining_bricks);
							break;
						case sf::Keyboard::N:
							loadNextLevel();
							break;
						case sf::Keyboard::Escape:
							setStatus(PAUSED);
							break;
						default:
							break;
					}
					break;
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Right)
					{
						createBall();
					}
					break;
				default:
					break;
			}
			break;

		case PAUSED:
			switch (m_menu.onEvent(event))
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
			if (event.type == sf::Event::MouseButtonPressed)
				Game::getInstance().previousScreen();
			break;
	}
}


void Wallbreaker::update(float frametime)
{
	if (m_status == PLAYING || m_status == READY)
	{
		Easing::update(frametime);

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
	target.draw(m_hud_sprite, states);
	target.draw(m_level_sprite, states);
	if (m_status == PAUSED)
	{
		m_menu.draw();
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

	if (m_status != PLAYING)
	{
		sf::RectangleShape overlay({m_width, m_height});
		overlay.setFillColor({0, 0, 0, 192});
		m_level_texture.draw(overlay);
		m_level_texture.draw(m_info_text);
	}
	m_level_texture.display();
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
			else if (entity.getY() + entity.getHeight() > m_height)
			{
				entity.kill();
			}
			// Check if entity collides with paddle
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
				int right =  (int) (entity.getX() + entity.getWidth())  / Brick::WIDTH;
				int bottom = (int) (entity.getY() + entity.getHeight()) / Brick::HEIGHT;

				if (checkBrick(entity, top, left, old_pos)
				 || checkBrick(entity, top, right, old_pos)
				 || checkBrick(entity, bottom, left, old_pos)
				 || checkBrick(entity, bottom, right, old_pos))
				{
					if (m_remaining_bricks == 0)
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


bool Wallbreaker::checkBrick(Entity& entity, int i, int j, const sf::Vector2f& old_pos)
{
	Brick& brick = m_level.getBrick(i, j);
	if (brick.isActive())
	{
		entity.setPosition(old_pos);
		entity.onBrickHit(brick);

		if (!brick.isActive())
		{
			--m_remaining_bricks;
			HUD::getInstance().setBrickCount(m_remaining_bricks);
		}
		return true;
	}
	return false;
}


bool Wallbreaker::loadNextLevel()
{
	m_remaining_bricks = m_level.loadNext();
	HUD::getInstance().setLevel(m_level.getCurrentLevel());
	HUD::getInstance().setBrickCount(m_remaining_bricks);
	SoundSystem::playSound("level-complete.ogg");
	Easing::stopAll();
	return m_remaining_bricks > 0; // No brick found = no more level
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
	}
	else if (status == PAUSED)
	{
		m_info_text.setString("Paused");
		m_info_text.setScale(2, 2);
		m_info_text.setPosition((m_width - m_info_text.getSize().x) / 2, 60);
	}
	Game::getInstance().getWindow().setMouseCursorVisible(status == PAUSED);
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
