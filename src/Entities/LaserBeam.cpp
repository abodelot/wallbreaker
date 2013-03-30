#include "LaserBeam.hpp"
#include "Brick.hpp"
#include "Core/Resources.hpp"


LaserBeam::LaserBeam()
{
	setTexture(Resources::getTexture("laserbeam.png"));
}


void LaserBeam::onUpdate(float frametime)
{
	float delta = 100 * frametime;
	move(0, -delta);
}


void LaserBeam::onBrickHit(Brick& brick, const sf::Vector2f& previous_pos)
{
	brick.takeDamage();
	kill();
}


void LaserBeam::onCeilHit()
{
	kill();
}
