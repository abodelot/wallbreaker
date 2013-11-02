#ifndef BRICK_HPP
#define BRICK_HPP

#include "Core/ParticleSystem.hpp"

class Brick: public sf::Sprite
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

	static const int HEIGHT = 8;
	static const int WIDTH  = 16;

	Brick();

	/**
	 * Set the brick type
	 */
	void setType(int id);
	inline Type getType() const { return m_type; }

	void setPosition(int x, int y);

	bool isActive() const;

	bool takeDamage(bool force_destruction=false);

	/**
	 * Play a sound according to the brick type
	 */
	void playSound();

private:
	/**
	 * Get color associated to the birck type
	 */
	sf::Color getBaseColor() const;

	Type m_type;
	bool m_broken;
	ParticleSystem::Emitter m_emitter;
};

#endif // BRICK_HPP
