#include "PowerUp.hpp"
#include "Brick.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
#include "Entities/Context.hpp"
#include "Paddle.hpp"
#include "States/Wallbreaker.hpp"
#include "Utils/Math.hpp"

static constexpr int POWERUP_WIDTH = 20;
static constexpr int POWERUP_HEIGHT = 12;
static constexpr int POWERUP_SPEED = 50;

PowerUp* PowerUp::createRandom()
{
    return new PowerUp(static_cast<Type>(math::rand(0, SPEED_RESET)));
}


PowerUp::PowerUp(Type type):
    m_type(type)
{
    int x = (type % 4) * POWERUP_WIDTH;
    int y = (type / 4) * POWERUP_HEIGHT;

    setTextureRect({x, y, POWERUP_WIDTH, POWERUP_HEIGHT});
    setTexture(Resources::getTexture("power-ups.png"));
}


void PowerUp::onPaddleHit(Paddle& paddle)
{
    switch (m_type)
    {
    case SMALL_PADDLE:
        paddle.shrink();
        break;
    case LARGE_PADDLE:
        paddle.grow();
        break;
    case STICKY_PADDLE:
        paddle.activeSticky();
        break;
    case LASER_PADDLE:
        paddle.activeLaser();
        break;
    case TRIPLE_BALL:
        Context::get().wallbreaker->createBall();
        Context::get().wallbreaker->createBall();
        break;
    case POWER_BALL:
        Context::get().wallbreaker->applyOnEachBall(&Ball::enablePowerBall);
        break;
    case EXTRA_LIFE:
        Context::get().wallbreaker->addPlayerLife();
        break;
    case SPEED_RESET:
        Context::get().wallbreaker->applyOnEachBall(&Ball::resetSpeed);
        break;
    }
    kill();
    SoundSystem::playSound("power-up.ogg");
}


void PowerUp::onUpdate(float frametime)
{
    move(0, POWERUP_SPEED * frametime);
}
