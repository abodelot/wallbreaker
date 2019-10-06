#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"
#include "Core/ParticleEmitter.hpp"


class Ball: public Entity
{
public:
    typedef void(Ball::*ActionPointer)(void);

    Ball();
    ~Ball();

    /**
     * Get number of allocated balls
     */
    static int getCount() { return s_instance_count; };

    Ball* toBall();

    /**
     * Unstick ball if glued to a paddle
     */
    void unstick();

    void enablePowerBall();

    void resetSpeed();

    void createParticles();

    // callbacks ---------------------------------------------------------------

    void onUpdate(float frametime) override;

    void onBrickHit(Brick& brick, const sf::Vector2f& previous_pos) override;

    void onPaddleHit(Paddle& paddle) override;

    void onWallHit() override;

    void onCeilHit() override;

private:
    float   m_angle;
    float   m_velocity;
    int     m_powerHits;
    // If ball is glued to a sticky paddle
    Paddle* m_gluedTo;
    float   m_gluedAt;

    ParticleEmitter m_emitter;

    static int s_instance_count;
};

#endif // BALL_HPP
