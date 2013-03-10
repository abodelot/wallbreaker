#include "PowerUp.hpp"
#include "PlayerPad.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"


PowerUp::PowerUp(Type type):
	m_type(type)
{
	setTextureRect(sf::IntRect((type) * SIZE, 0, SIZE, SIZE));
	setTexture(Resources::getTexture("power-ups.png"));
}


void PowerUp::onCollide(const PlayerPad& pad)
{
	switch (m_type)
	{
		case EXTRA_BALL:
			break;
	}
	kill();
	SoundSystem::playSound("power-up.ogg");
}


void PowerUp::onUpdate(float frametime)
{
	move(0, 50 * frametime);
}
