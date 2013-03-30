#ifndef LASER_BEAM_HPP
#define LASER_BEAM_HPP

#include "Entity.hpp"

class LaserBeam: public Entity
{
public:
	LaserBeam();

	void onUpdate(float frametime);

	void onBrickHit(Brick& brick, const sf::Vector2f& previous_pos);

	void onCeilHit();
};

#endif // LASER_BEAM_HPP
