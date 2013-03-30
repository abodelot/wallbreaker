#include <SFML/Window.hpp>

#include "Paddle.hpp"
#include "LaserBeam.hpp"
#include "Ball.hpp"
#include "Core/Game.hpp"
#include "Core/SoundSystem.hpp"
#include "Core/Resources.hpp"
#include "Screens/Wallbreaker.hpp"
#include "Utils/Math.hpp"


Paddle::Paddle():
	m_sticky(false)
{
	setTexture(Resources::getTexture("paddles.png"));
	setType(MEDIUM);
}


void Paddle::onUpdate(float frametime)
{
	// Paddle follows the mouse cursor
	const sf::RenderWindow& window = Game::getInstance().getWindow();
	int x = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x;
	setPosition(x - getWidth() / 2, getPosition().y);
}


void Paddle::grow()
{
	if (m_type == SMALL)
		setType(MEDIUM);
	else
		setType(LARGE);
}


void Paddle::shrink()
{
	if (m_type == LARGE)
		setType(MEDIUM);
	else
		setType(SMALL);
}


void Paddle::reset()
{
	setType(MEDIUM);
	m_sticky = false;
}


void Paddle::activeSticky()
{
	m_sticky = true;
	if (m_type == LASER)
		setType(MEDIUM);
}


bool Paddle::isSticky() const
{
	return m_sticky;
}


void Paddle::activeLaser()
{
	setType(LASER);
	if (m_sticky)
	{
		m_sticky = false;
		getParent()->applyOnEachBall(&Ball::unstick);
	}
}


bool Paddle::hasLaser() const
{
	return m_type == LASER;
}


LaserBeam* Paddle::shoot() const
{
	LaserBeam* laserbeam = new LaserBeam();
	laserbeam->setPosition(getX() + (getWidth() - laserbeam->getWidth()) / 2, getY());
	SoundSystem::playSound("laser.ogg", math::rand(1.f, 2.f));
	return laserbeam;
}


void Paddle::setType(Type type)
{
	sf::IntRect subrect;
	switch (type)
	{
		case SMALL:
			subrect = sf::IntRect(0, 0, 24, 16);
			break;
		case MEDIUM:
			subrect = sf::IntRect(0, 16, 32, 16);
			break;
		case LARGE:
			subrect = sf::IntRect(0, 32, 48, 16);
			break;
		case LASER:
			subrect = sf::IntRect(0, 48, 32, 16);
			break;
	}
	setTextureRect(subrect);
	m_type = type;
}

