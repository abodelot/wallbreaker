#include "Ball.hpp"
#include "Brick.hpp"
#include "Paddle.hpp"
#include "Core/Effect.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
#include "Utils/Math.hpp"

// When hitting the pad, the ball bounces from PADDLE_ANGLE to 90 + PADDLE_ANGLE
#define PADDLE_ANGLE           30 // degrees
#define BALL_START_SPEED      150 // pixels/second
#define BALL_MAX_SPEED        450 // pixels/second
#define BALL_SPEED_STEP         5 // pixels/second
#define POWER_BALL_COUNT       10 // nb bricks

int Ball::s_instance_count = 0;

Ball::Ball():
	m_angle(math::to_radians(math::rand(PADDLE_ANGLE, 90 + PADDLE_ANGLE))),
	m_velocity(BALL_START_SPEED),
	m_powered(0),
	m_glued_to(NULL)
{
	setTexture(Resources::getTexture("balls.png"));
	setTextureRect({0, 0, 8, 8});
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


Ball* Ball::toBall()
{
	return this;
}


void Ball::unstick()
{
	m_glued_to = NULL;
}


void Ball::enablePowerBall()
{
	m_powered = POWER_BALL_COUNT;
	setTextureRect({8, 0, 8, 8});
	Emitter::m_color = sf::Color::Cyan;
	Emitter::m_to_color = sf::Color(0, 0, 255, 0);
}


void Ball::resetSpeed()
{
	m_velocity = BALL_START_SPEED;
}


void Ball::onUpdate(float frametime)
{
	if (m_glued_to == NULL)
	{
		float delta = m_velocity * frametime;
		move(delta * std::cos(m_angle), -delta * std::sin(m_angle));
	}
	else
	{
		// Stick to the paddle
		setX(m_glued_to->getX() - m_glued_at);
	}
}


void Ball::onWallHit()
{
	if (m_glued_to == NULL)
	{
		m_angle = math::PI - m_angle;
		SoundSystem::playSound("ball.ogg", 0.8f);
	}
}


void Ball::onCeilHit()
{
	if (m_glued_to == NULL)
	{
		m_angle = -m_angle;
		SoundSystem::playSound("ball.ogg", 0.6f);
	}
}


void Ball::onBrickHit(Brick& brick, const sf::Vector2f& previous_pos)
{
	// Increase ball speed if brick was destroyed
	if (brick.takeDamage(m_powered > 0) && m_velocity < BALL_MAX_SPEED)
		m_velocity += BALL_SPEED_STEP;

	if (m_powered > 0 && brick.getType() != Brick::UNBREAKABLE)
	{
		--m_powered;
		if (m_powered == 0)
		{
			// Turn off the Power-ball
			m_powered = false;
			setTextureRect({0, 0, 8, 8});
			Emitter::m_color = sf::Color::Red;
			Emitter::m_to_color = sf::Color(255, 255, 0, 0);
		}
		return;
	}

	// Get the side of the brick hit by the ball
	sf::Vector2f ball_center = previous_pos;
	ball_center.x += getWidth() / 2;
	ball_center.y += getHeight() / 2;

	sf::Vector2f brick_center = brick.getPosition();
	brick_center.x += Brick::WIDTH / 2;
	brick_center.y += Brick::HEIGHT / 2;

	// Angle between the ball and the brick before the collision occured
	float angle_ball = math::to_degrees(math::angle(ball_center, brick_center));

	// Get brick ratio angle
	float brick_ratio = math::to_degrees(std::tan((float) Brick::HEIGHT / Brick::WIDTH));

	// Check if collision was with a vertical side
	if ((angle_ball > brick_ratio       && angle_ball < (180 - brick_ratio)) ||
	    (angle_ball > 180 + brick_ratio && angle_ball < (360 - brick_ratio)))
	{
		// Vertical side, flip Y-axis
		m_angle = -m_angle;
	}
	else
	{
		// Horizontal side, flip X-axis
		m_angle = math::PI - m_angle;
	}

	setPosition(previous_pos);
}


void Ball::onPaddleHit(Paddle& paddle)
{
	float x = getX() + getWidth() / 2 - paddle.getX();
	float range = 180 - PADDLE_ANGLE * 2;
	float degrees = (range * x / paddle.getWidth()) + PADDLE_ANGLE;

	m_angle = math::PI - math::to_radians(degrees); // Angles are inverted, flip Y
	SoundSystem::playSound("ball.ogg", 0.4f);

	if (paddle.isSticky())
	{
		m_glued_to = &paddle;
		m_glued_at = paddle.getX() - getX();
	}
	else
	{
		Effect::zoomAndRevert(*this, 1.8, 0.1);
	}
}


sf::Vector2f Ball::getSpawnPosition() const
{
	sf::Vector2f center = getPosition();
	center.x += getWidth() / 2;
	center.y += getHeight() / 2;
	return center;
}

