#ifndef POWERUP_HPP
#define POWERUP_HPP

#include "Entity.hpp"

class PlayerPad;

class PowerUp: public Entity
{
public:
	enum Type
	{
		TRIPLE_BALL,
		POWER_BALL,
		SMALL_PADDLE,
		LARGE_PADDLE,
		STICKY_PADDLE,
		EXTRA_LIFE
	};
	static const int WIDTH = 20;
	static const int HEIGHT = 10;

	/**
	 * Alocate a new power up with a random type
	 */
	static PowerUp* createRandom();

	PowerUp(Type type);

	void onCollide(const Paddle& paddle);

	void onUpdate(float frametime);

private:
	Type m_type;
};

#endif // POWERUP_HPP
