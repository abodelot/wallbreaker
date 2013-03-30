#ifndef POWERUP_HPP
#define POWERUP_HPP

#include "Entity.hpp"

class PlayerPad;

class PowerUp: public Entity
{
public:
	enum Type
	{
		SMALL_PADDLE,
		LARGE_PADDLE,
		STICKY_PADDLE,
		LASER_PADDLE,
		TRIPLE_BALL,
		POWER_BALL,
		EXTRA_LIFE,
		RANDOM
	};

	/**
	 * Alocate a new power up with a random type
	 */
	static PowerUp* createRandom();

	PowerUp(Type type);

	void onPaddleHit(Paddle& paddle);

	void onUpdate(float frametime);

private:
	Type m_type;
};

#endif // POWERUP_HPP
