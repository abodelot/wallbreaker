#include <SFML/Window.hpp>

#include "Paddle.hpp"
#include "Core/Game.hpp"
#include "Core/Resources.hpp"


Paddle::Paddle():
	m_sticky(false)
{
	setTexture(Resources::getTexture("paddles.png"));
	setSize(MEDIUM);
}


void Paddle::onUpdate(float frametime)
{
	// Paddle follows the mouse cursor
	const sf::RenderWindow& window = Game::getInstance().getWindow();
	int x = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x;
	setPosition(x - getWidth() / 2, getPosition().y);
}


void Paddle::grow()
{
	if (m_size < LARGE)
		setSize((Size) (m_size + 1));
}


void Paddle::shrink()
{
	if (m_size > 0)
		setSize((Size) (m_size - 1));
}


void Paddle::reset()
{
	setSize(MEDIUM);
	setSticky(false);
}


void Paddle::setSticky(bool sticky)
{
	m_sticky = sticky;
}


bool Paddle::isSticky() const
{
	return m_sticky;
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
			subrect = sf::IntRect(0, 32, 48, 16);
			break;
	}
	setTextureRect(subrect);
	m_size = size;
}

