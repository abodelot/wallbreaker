#include "LaserBeam.hpp"
#include "Brick.hpp"
#include "Core/Resources.hpp"


LaserBeam::LaserBeam()
{
    setTexture(Resources::getTexture("laserbeam.png"));
}


void LaserBeam::onUpdate(float frametime)
{
    const float delta = 150 * frametime;
    move(0, -delta);
}


void LaserBeam::onBrickHit(Brick& brick, const sf::Vector2f&)
{
    brick.takeDamage();
    kill();
}


void LaserBeam::onCeilHit()
{
    kill();
}
