#ifndef LASER_BEAM_HPP
#define LASER_BEAM_HPP

#include "Entity.hpp"

class LaserBeam: public Entity
{
public:
	LaserBeam();

	void onUpdate(float frametime);

	void onCeilHit();

private:
	float m_length;
};

#endif // LASER_BEAM_HPP
