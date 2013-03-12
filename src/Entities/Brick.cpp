#include "Brick.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
#include "Core/Easing.hpp"

#define FADE_DELAY 2

#include <iostream>

Brick::Brick():
	m_type(BRICK_NONE),
	m_broken(false)
{
	setTexture(Resources::getTexture("bricks.png"));
	setParticleCount(20);
	Emitter::m_type = Emitter::TRAIL;
	Emitter::m_speed = 15;
	Emitter::m_speed_variation = 5;
	Emitter::m_angle = 0.f;
	Emitter::m_angle_variation = 2 * math::PI;
	Emitter::m_time_to_live = 5.f;
	Emitter::setColor(sf::Color::Green);
}


void Brick::setType(int id)
{

	if (id < BRICK_START || id > BRICK_UNBREAKABLE)
		id = BRICK_NONE;

	m_type = id;
	m_broken = false;
	if (id != BRICK_NONE)
	{
		int tile_id = (id - BRICK_START);
		// 4 bricks per row
		int x = (tile_id % 4) * Brick::WIDTH;
		int y = (tile_id / 4) * Brick::HEIGHT;

		sf::Sprite::setColor(sf::Color::White);
		setTextureRect(sf::IntRect(x, y, Brick::WIDTH, Brick::HEIGHT));

		Emitter::setColor(getBaseColor());
	}
}


bool Brick::isActive() const
{
	return m_type != BRICK_NONE && !m_broken;
}


bool Brick::takeDamage(bool force_destruction)
{
	// The higher the ID, the higher the pitch
	float pitch = 1.f + (float)(m_type - BRICK_START) / 8;
	SoundSystem::playSound("ball.ogg", pitch);

	switch (m_type)
	{
		case BRICK_UNBREAKABLE:
			break;
		case BRICK_ROCK:
			setType(BRICK_ROCK_2);
			break;
		case BRICK_ROCK_2:
			setType(BRICK_ROCK_3);
			break;
		default:
			m_broken = true;
			Easing::move(*this, {getPosition().x, getPosition().y + math::rand(20, 40)}, 1);
			Easing::fadeOut(*this, 1);
			Easing::rotate(*this, math::rand(-60, 60), 1);
			Easing::scale(*this, 1, 2, 1);
			launchParticles();
			break;
	}
	return m_broken;
}


sf::Vector2f Brick::getSpawnPosition() const
{
	sf::Vector2f pos = getPosition();
	pos.x += WIDTH / 2;
	pos.y += HEIGHT / 2;
	return pos;
}


sf::Color Brick::getBaseColor() const
{
	switch (m_type - BRICK_START)
	{
		case 0: return sf::Color(255, 0, 0);
		case 1: return sf::Color(255, 0, 128);
		case 2: return sf::Color(255, 0, 255);
		case 3: return sf::Color(128, 0, 255);

		case 4: return sf::Color(0, 0, 255);
		case 5: return sf::Color(0, 128, 255);
		case 6: return sf::Color(0, 255, 255);
		case 7: return sf::Color(0, 255, 128);

		case 8:  return sf::Color(0, 255, 0);
		case 9:  return sf::Color(128, 255, 0);
		case 10: return sf::Color(255, 255, 0);
		case 11: return sf::Color(255, 128, 0);
	}
	return sf::Color::White;
}
