#ifndef POWERUP_HPP
#define POWERUP_HPP

#include "Entity.hpp"

class PlayerPad;

class PowerUp: public Entity
{
public:
	enum Type
	{
		EXTRA_BALL
	};
	static const int SIZE = 16;

	PowerUp(Type type);

	void onCollide(const PlayerPad& pad);

	void onUpdate(float frametime);

private:
	Type m_type;
};

#endif // POWERUP_HPP
