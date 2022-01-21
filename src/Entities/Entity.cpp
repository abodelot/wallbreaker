#include "Entity.hpp"
#include "States/Wallbreaker.hpp"


Entity::Entity():
    m_alive(true)
{
}


Entity::~Entity()
{
}


bool Entity::isAlive() const
{
    return m_alive;
}


void Entity::kill()
{
    m_alive = false;
}


bool Entity::collidesWith(const Entity& other) const
{
    sf::FloatRect a(getX(), getY(), getWidth(), getHeight());
    sf::FloatRect b(other.getX(), other.getY(), other.getWidth(), other.getHeight());
    return a.intersects(b);
}


Ball* Entity::toBall()
{
    return nullptr;
}


sf::Vector2f Entity::getCenter() const
{
    sf::Vector2f pos = getPosition() - getOrigin();
    pos.x += getTextureRect().width / 2;
    pos.y += getTextureRect().height / 2;
    return pos;
}


sf::IntRect Entity::getCollisionRect() const
{
    sf::IntRect rect = getTextureRect();
    rect.left = getPosition().x;
    rect.top = getPosition().y;
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
