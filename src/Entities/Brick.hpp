#ifndef BRICK_HPP
#define BRICK_HPP

#include "Core/ParticleSystem.hpp"

class Brick: public sf::Sprite, public ParticleSystem::Emitter
{
public:
	enum Type
	{
		NONE        = '.',
		START       = 'a',
		ROCK        = 'i',
		ROCK_2      = 'j',
		ROCK_3      = 'k',
		UNBREAKABLE = 'l'
	};

	static const int HEIGHT = 10;
	static const int WIDTH  = 20;

	Brick();

	// The bricks type
	void setType(int id);
	inline Type getType() const { return m_type; }

	bool isActive() const;

	// override
	sf::Vector2f getSpawnPosition() const;

	bool takeDamage(bool force_destruction=false);

	/**
	 * Play a sound according to the brick type
	 */
	void playSound();

private:
	sf::Color getBaseColor() const;
	Type m_type;
	bool m_broken;
};

#endif // BRICK_HPP
