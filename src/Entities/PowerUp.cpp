#include "PowerUp.hpp"
#include "Paddle.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
#include "Screens/Wallbreaker.hpp"
#include "Utils/Math.hpp"


PowerUp* PowerUp::createRandom()
{
	return new PowerUp((Type) math::rand(0, EXTRA_LIFE));
}


PowerUp::PowerUp(Type type):
	m_type(type)
{
	setTextureRect(sf::IntRect((type) * WIDTH, 0, WIDTH, HEIGHT));
	setTexture(Resources::getTexture("power-ups.png"));
}


void PowerUp::onPaddleHit(Paddle& paddle)
{
	switch (m_type)
	{
		case TRIPLE_BALL:
			getParent()->createBall();
			getParent()->createBall();
			break;
		case POWER_BALL:
			getParent()->applyOnEachBall(&Ball::activePower);
			break;
		case SMALL_PADDLE:
			paddle.shrink();
			break;
		case LARGE_PADDLE:
			paddle.grow();
			break;
		case STICKY_PADDLE:
			paddle.setSticky(true);
			break;
		case EXTRA_LIFE:
			getParent()->addPlayerLife();
			break;
	}
	kill();
	SoundSystem::playSound("power-up.ogg");
}


void PowerUp::onUpdate(float frametime)
{
	move(0, 50 * frametime);
}
