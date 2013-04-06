#include "PowerUp.hpp"
#include "Paddle.hpp"
#include "Brick.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
#include "Screens/Wallbreaker.hpp"
#include "Utils/Math.hpp"


PowerUp* PowerUp::createRandom()
{
	return new PowerUp((Type) math::rand(0, BLACKOUT));
}


PowerUp::PowerUp(Type type):
	m_type(type)
{
	int x = (type % 4) * Brick::WIDTH;
	int y = (type / 4) * Brick::HEIGHT;

	setTextureRect({x, y, Brick::WIDTH, Brick::HEIGHT});
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
			getParent()->createBall();
			getParent()->createBall();
			break;
		case POWER_BALL:
			getParent()->applyOnEachBall(&Ball::activePower);
			break;
		case EXTRA_LIFE:
			getParent()->addPlayerLife();
			break;
		case BLACKOUT:
			getParent()->blackout();
			break;
	}
	kill();
	SoundSystem::playSound("power-up.ogg");
}


void PowerUp::onUpdate(float frametime)
{
	move(0, 50 * frametime);
}
