#include <SFML/Window.hpp>

#include "Paddle.hpp"
#include "Core/Game.hpp"
#include "Core/Resources.hpp"



Paddle::Paddle()
{
	setTexture(Resources::getTexture("pads.png"));
	setSize(MEDIUM);
}


void Paddle::onUpdate(float frametime)
{
	const sf::RenderWindow& window = Game::getInstance().getWindow();
	int x = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x;
	setPosition(x - getWidth() / 2, getPosition().y);
}


void Paddle::setSize(Size size)
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
	m_size = size;
}

