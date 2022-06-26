#include <SFML/Window.hpp>

#include "Ball.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
#include "Entities/Context.hpp"
#include "LaserBeam.hpp"
#include "Paddle.hpp"
#include "States/Wallbreaker.hpp"
#include "Utils/Math.hpp"

// Paddle speed when moved with arrow keys
#define PADDLE_SPEED 150

Paddle::Paddle():
    m_sticky(false)
{
    setTexture(Resources::getTexture("paddles.png"));
    setType(MEDIUM);
}


void Paddle::onMouseMoved(const sf::Vector2f& mouse)
{
    // Paddle follows the mouse cursor
    setPosition(mouse.x - getWidth() / 2, getPosition().y);
}


void Paddle::onUpdate(float frametime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        move(-PADDLE_SPEED * frametime, 0);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        move(PADDLE_SPEED * frametime, 0);
    }
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
        Context::get().wallbreaker->applyOnEachBall(&Ball::unstick);
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
