#include "LaserBeam.hpp"
#include "Core/Resources.hpp"
#include <cstdio>

LaserBeam::LaserBeam():
	m_length(0)
{
	setTexture(Resources::getTexture("laser.gif"));
}


void LaserBeam::onUpdate(float frametime)
{
	float delta = 100 * frametime;
	move(0, -delta);
}


void LaserBeam::onCeilHit()
{
	kill();
}
