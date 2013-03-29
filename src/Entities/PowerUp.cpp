#include "PowerUp.hpp"
#include "Paddle.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
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


void PowerUp::onCollide(const Paddle& paddle)
{

	kill();
	SoundSystem::playSound("power-up.ogg");
}


void PowerUp::onUpdate(float frametime)
{
	move(0, 40 * frametime);
}
