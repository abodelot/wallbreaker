#include "Ball.hpp"
#include "Brick.hpp"
#include "Paddle.hpp"
#include "Core/Effect.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
#include "Utils/Math.hpp"

// When hitting the pad, the ball bounces from PADDLE_ANGLE to 90 + PADDLE_ANGLE
#define PADDLE_ANGLE           30 // degrees
// The ball velocity starts at BALL_SPEED_STEP, and is increased by BALL_SPEED_STEP
// every time a brick is hit, until BALL_MAX_SPEED is reached
#define BALL_START_SPEED      150 // pixels/second
#define BALL_MAX_SPEED        300 // pixels/second
#define BALL_SPEED_STEP         3 // pixels/second
// Remaining number of bricks when Power Ball power-up is activated
#define POWER_BALL_COUNT       10

int Ball::s_instance_count = 0;

Ball::Ball():
    m_angle(math::to_radians(math::rand(PADDLE_ANGLE, 90 + PADDLE_ANGLE))),
    m_velocity(BALL_START_SPEED),
    m_powerHits(0),
    m_gluedTo(nullptr)
{
    setTexture(Resources::getTexture("balls.png"));
    setTextureRect({0, 0, 8, 8});
    ++s_instance_count;

    m_emitter.setParticleCount(100);
    m_emitter.setParticleColor(sf::Color::Red, sf::Color(255, 255, 0, 0));
    m_emitter.setSpeed(10, 5);
    m_emitter.setAngle(0, math::PI * 2);
    m_emitter.setTimeToLive(1.f);
    m_emitter.setLooping(true);
}


Ball::~Ball()
{
    m_emitter.clearParticles();
    --s_instance_count;
}


Ball* Ball::toBall()
{
    return this;
}


void Ball::unstick()
{
    m_gluedTo = nullptr;
}


void Ball::enablePowerBall()
{
    m_powerHits = POWER_BALL_COUNT;
    setTextureRect({8, 0, 8, 8});
    m_emitter.setParticleColor(sf::Color::Cyan, sf::Color(0, 0, 255, 0));
}


void Ball::resetSpeed()
{
    m_velocity = BALL_START_SPEED;
}


void Ball::onInit()
{
    m_emitter.setSpawnPosition(getCenter());
    m_emitter.launchParticles();
}


void Ball::onUpdate(float frametime)
{
    if (m_gluedTo == nullptr)
    {
        float delta = m_velocity * frametime;
        move(delta * std::cos(m_angle), -delta * std::sin(m_angle));
    }
    else
    {
        // Stick to the paddle
        setX(m_gluedTo->getX() - m_gluedAt);
    }
    m_emitter.setSpawnPosition(getCenter());
}


void Ball::onWallHit()
{
    if (m_gluedTo == nullptr)
    {
        m_angle = math::PI - m_angle;
        SoundSystem::playSound("ball.ogg", 0.8f);
    }
}


void Ball::onCeilHit()
{
    if (m_gluedTo == nullptr)
    {
        m_angle = -m_angle;
        SoundSystem::playSound("ball.ogg", 0.6f);
    }
}


void Ball::onBrickHit(Brick& brick, const sf::Vector2f& previousPos)
{
    // Increase ball speed if brick was destroyed
    if (brick.takeDamage(m_powerHits > 0) && m_velocity < BALL_MAX_SPEED)
        m_velocity += BALL_SPEED_STEP;

    if (m_powerHits > 0 && brick.getType() != Brick::UNBREAKABLE)
    {
        --m_powerHits;
        if (m_powerHits == 0)
        {
            // Turn off the Power-ball
            setTextureRect({0, 0, 8, 8});
            m_emitter.setParticleColor(sf::Color::Red, sf::Color(255, 255, 0, 0));
        }
        return;
    }

    // Get the side of the brick hit by the ball
    const sf::Vector2f ballCenter(
        previousPos.x + getWidth() / 2,
        previousPos.y + getHeight() / 2
    );
    const sf::Vector2f brickCenter(
        brick.getPosition().x + Brick::WIDTH / 2,
        brick.getPosition().y + Brick::HEIGHT / 2
    );

    // Angle between the ball and the brick before the collision occured
    float ballAngle = math::to_degrees(math::angle(ballCenter, brickCenter));

    // Get brick ratio angle
    float brickRatio = math::to_degrees(std::tan((float) Brick::HEIGHT / Brick::WIDTH));

    // Check if collision was with a vertical side
    if ((ballAngle > brickRatio       && ballAngle < (180 - brickRatio)) ||
        (ballAngle > 180 + brickRatio && ballAngle < (360 - brickRatio)))
    {
        // Vertical side, flip Y-axis
        m_angle = -m_angle;
    }
    else
    {
        // Horizontal side, flip X-axis
        m_angle = math::PI - m_angle;
    }
    setPosition(previousPos);
}


void Ball::onPaddleHit(Paddle& paddle)
{
    const float x = getX() + getWidth() / 2 - paddle.getX();
    const float range = 180 - PADDLE_ANGLE * 2;
    const float degrees = (range * x / paddle.getWidth()) + PADDLE_ANGLE;

    m_angle = math::PI - math::to_radians(degrees); // Angles are inverted, flip Y
    SoundSystem::playSound("ball.ogg", 0.4f);

    if (paddle.isSticky())
    {
        m_gluedTo = &paddle;
        m_gluedAt = paddle.getX() - getX();
    }
    else
    {
        Effect::zoomAndRevert(*this, 1.8, 0.1);
    }
}
