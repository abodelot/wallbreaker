#ifndef PADDLE_HPP
#define PADDLE_HPP

#include "Entity.hpp"

class LaserBeam;

class Paddle: public Entity
{
public:
    Paddle();

    void onMouseMoved(const sf::Vector2f& mouse);

    void onUpdate(float frametime) override;

    /**
     * Increase paddle size
     */
    void grow();

    /**
     * Decrease paddle size
     */
    void shrink();

    /**
     * Reset paddle to its default behaviour
     */
    void reset();

    /**
     * Turn on glue
     */
    void activeSticky();
    bool isSticky() const;

    /**
     * Turn on laser
     */
    void activeLaser();
    bool hasLaser() const;

    LaserBeam* shoot() const;

private:
    enum Type
    {
        SMALL, MEDIUM, LARGE, LASER
    };

    void setType(Type size);

    Type m_type;
    bool m_sticky;
};

#endif // PADDLE_HPP
