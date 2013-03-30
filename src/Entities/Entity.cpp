#include "Entity.hpp"
#include "Screens/Wallbreaker.hpp"


Entity::Entity():
	m_parent(NULL),
	m_alive(true)
{
}


Entity::~Entity()
{
}


void Entity::setParent(Wallbreaker* parent)
{
	m_parent = parent;
}


Wallbreaker* Entity::getParent()
{
	return m_parent;
}


bool Entity::collidesWith(const Entity& other) const
{
	sf::FloatRect a(getPosition().x, getPosition().y, getWidth(), getHeight());
	sf::FloatRect b(other.getPosition().x, other.getPosition().y, other.getWidth(), other.getHeight());

	return a.intersects(b);
}


Ball* Entity::toBall()
{
	return NULL;
}


sf::IntRect Entity::getCollisionRect() const
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
