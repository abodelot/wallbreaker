#include "Entity.hpp"


Entity::Entity():
	m_alive(true)
{
}


bool Entity::collidesWith(const Entity& other) const
{
	sf::FloatRect a(getPosition().x, getPosition().y, getWidth(), getHeight());
	sf::FloatRect b(other.getPosition().x, other.getPosition().y, other.getWidth(), other.getHeight());

	return a.intersects(b);
}


sf::IntRect Entity::getSurface() const
{
	sf::IntRect rect = getTextureRect();
	rect.left = getPosition().x;
	rect.top  = getPosition().y;
	return rect;
}


float Entity::getWidth() const
{
	return getTextureRect().width;
}


float Entity::getHeight() const
{
	return getTextureRect().height;
}
