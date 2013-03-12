#include "Ball.hpp"
#include "Brick.hpp"
#include "PlayerPad.hpp"
#include "Core/Easing.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
#include "Utils/Math.hpp"

// When hitting the pad, the ball bounces from -PAD_ANGLE to +PAD_ANGLE
#define PAD_ANGLE 45


int Ball::s_instance_count = 0;

Ball::Ball():
	m_angle(math::PI / 2),
	m_velocity(140)
{
	setTexture(Resources::getTexture("ball.png"));
	++s_instance_count;

	setParticleCount(100);
	Emitter::m_color = sf::Color::Red;
	Emitter::m_to_color = sf::Color(255, 255, 0, 0);
	Emitter::m_angle_variation = math::PI * 2;
	Emitter::m_time_to_live = 1;
	Emitter::m_speed = 10;
	Emitter::m_speed_variation = 5;
	Emitter::m_looping = true;
}


Ball::~Ball()
{
	Emitter::clearParticles();
	--s_instance_count;
}


void Ball::onUpdate(float frametime)
{
	float delta = m_velocity * frametime;
	move(delta * std::cos(m_angle), -delta * std::sin(m_angle));
}


void Ball::onWallHit()
{
	m_angle = math::PI - m_angle;
	SoundSystem::playSound("wall.ogg");
	//ParticleSystem::Emitter::m_angle = m_angle;
	//spawn();
}


void Ball::onCeilHit()
{
	m_angle = -m_angle;
	SoundSystem::playSound("wall.ogg");
	//ParticleSystem::Emitter::m_angle = m_angle;
	//spawn();
	//Easing::scaleAndReset(*this, 1, 2, 0.2);
}


void Ball::onBrickHit(Brick& brick)
{
	if (brick.takeDamage())
		m_velocity += 5;

	sf::IntRect intersection;
	sf::IntRect brick_rect(brick.getPosition().x, brick.getPosition().y, Brick::WIDTH, Brick::HEIGHT);
	brick_rect.intersects(getCollisionRect(), intersection);

	if (intersection.height > intersection.width)
		onWallHit();
	else
		onCeilHit();
}


void Ball::onCollide(const PlayerPad& pad)
{
	float x = getPosition().x + getWidth() / 2 - pad.getPosition().x;
	float range = PAD_ANGLE * 2;
	float angle_diff = (range * x / (float) pad.getWidth()) - PAD_ANGLE;

	m_angle = math::to_rad(90 - angle_diff);
	//ParticleSystem::Emitter::m_angle = m_angle;
	//spawn();
	SoundSystem::playSound("pad.ogg");
	//Easing::scaleAndReset(*this, 1, 2, 0.2);
}


sf::Vector2f Ball::getSpawnPosition() const
{
	sf::Vector2f center = getPosition();
	center.x += getWidth() / 2;
	center.y += getHeight() / 2;
	return center;
}

