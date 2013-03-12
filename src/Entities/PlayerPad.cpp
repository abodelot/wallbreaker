#include <SFML/Window.hpp>

#include "PlayerPad.hpp"
#include "Core/Game.hpp"
#include "Core/Resources.hpp"



PlayerPad::PlayerPad()
{
	setTexture(Resources::getTexture("pads.png"));
	setPadSize(MEDIUM);
}


void PlayerPad::onUpdate(float frametime)
{
	const sf::RenderWindow& window = Game::getInstance().getWindow();
	int x = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x;
	setPosition(x - getWidth() / 2, getPosition().y);
}


void PlayerPad::setPadSize(PadSize size)
{
	sf::IntRect subrect;
	switch (size)
	{
		case SMALL:
			subrect = sf::IntRect(0, 0, 24, 16);
			break;
		case MEDIUM:
			subrect = sf::IntRect(0, 16, 32, 16);
			break;
		case LARGE:
			subrect = sf::IntRect(0, 32, 40, 16);
			break;
	}
	setTextureRect(subrect);
	m_pad_size = size;
}

