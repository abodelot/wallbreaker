#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics/Sprite.hpp>

class Ball;
class Paddle;
class Brick;

/**
 * Base class for game entities
 */
class Entity: public sf::Sprite
{
public:
    Entity();
    virtual ~Entity();

    bool isAlive() const;

    void kill();

    bool collidesWith(const Entity& other) const;

    // casts -------------------------------------------------------------------

    virtual Ball* toBall();

    // callbacks ---------------------------------------------------------------

    virtual void onUpdate(float frametime) = 0;

    virtual void onBrickHit(Brick&, const sf::Vector2f&) {};
    virtual void onPaddleHit(Paddle&) {}

    virtual void onCeilHit() {}
    virtual void onWallHit() {}

    // position helpers --------------------------------------------------------

    inline float getX() const { return getPosition().x; }
    inline float getY() const { return getPosition().y; }
    inline void setX(float x) { setPosition(x, getPosition().y); }
    inline void setY(float y) { setPosition(getPosition().x, y); }
    sf::Vector2f getCenter() const;

    // size helpers ------------------------------------------------------------

    float getWidth() const;
    float getHeight() const;
    sf::IntRect getCollisionRect() const;

private:
    bool m_alive;
};

#endif // ENTITY_HPP
