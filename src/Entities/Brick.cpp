#include "Brick.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
#include "Core/Effect.hpp"
#include "Utils/Math.hpp"


Brick::Brick():
	m_type(NONE),
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
}


void Brick::setType(int type)
{
	if (type < START || type > UNBREAKABLE)
		type = NONE;

	m_type = (Type) type;
	m_broken = false;
	if (type != NONE)
	{
		int tile_id = (type - START);
		// 4 bricks per row
		int x = (tile_id % 4) * Brick::WIDTH;
		int y = (tile_id / 4) * Brick::HEIGHT;

		setTextureRect(sf::IntRect(x, y, Brick::WIDTH, Brick::HEIGHT));
		setColor(sf::Color::White);
		setParticleColor(getBaseColor());
	}
}


bool Brick::isActive() const
{
	return m_type != NONE && !m_broken;
}


bool Brick::takeDamage(bool force_destruction)
{
	playSound();

	switch (m_type)
	{
		case UNBREAKABLE:
			return false;
		case ROCK:
			setType(ROCK_2);
			break;
		case ROCK_2:
			setType(ROCK_3);
			break;
		default:
			m_broken = true;
			break;
	}

	if (force_destruction)
		m_broken = true;

	if (m_broken)
	{
		Effect::move(*this, {0, math::rand(20.f, 40.f)});
		Effect::fadeOut(*this);
		Effect::rotate(*this, math::rand(-60, 60));
		Effect::zoom(*this, 0.5);
		launchParticles();
	}
	return m_broken;
}


void Brick::playSound()
{
	// The higher the ID, the higher the pitch
	float pitch = 0.7f + (float)(m_type - START) / 5;
	SoundSystem::playSound("ball.ogg", pitch);
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
	switch (m_type - START)
	{
		case 0: return sf::Color(0x2e, 0xcd, 0x71);
		case 1: return sf::Color(0x1b, 0xbc, 0x9b);
		case 2: return sf::Color(0x35, 0x98, 0xdb);
		case 3: return sf::Color(0x9b, 0x58, 0xb5);

		case 4: return sf::Color(0xe2, 0x5d, 0xb5);
		case 5: return sf::Color(0xe8, 0x4c, 0x3d);
		case 6: return sf::Color(0xe7, 0x7e, 0x23);
		case 7: return sf::Color(0xf1, 0xc4, 0x0f);

		default: return sf::Color(0xbe, 0xc3, 0xc7);
	}
}
