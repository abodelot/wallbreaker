#ifndef BRICK_HPP
#define BRICK_HPP


#include "Core/ParticleSystem.hpp"

#define BRICK_NONE 46
#define BRICK_START 97
#define BRICK_ROCK  105
#define BRICK_ROCK_2 106
#define BRICK_ROCK_3 107
#define BRICK_UNBREAKABLE 108

class Brick: public sf::Sprite, public ParticleSystem::Emitter
{
public:
	static const int HEIGHT = 10;
	static const int WIDTH  = 20;

	// Convert a character to a brick type
	static int charToBrickID(char c);

	Brick();

	// The bricks's type
	void setType(int id);
	inline int getType() const { return m_type; }

	bool isActive() const;


	// override
	sf::Vector2f getSpawnPosition() const;

	void takeDamage(bool force_destruction=false);

private:
	sf::Color getBaseColor() const;
	int  m_type;
	bool m_broken;
};

#endif // BRICK_HPP
