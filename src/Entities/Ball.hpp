#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"
#include "Core/ParticleSystem.hpp"


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

    // callbacks ---------------------------------------------------------------

    void onInit() override;

    void onUpdate(float frametime) override;

    void onBrickHit(Brick& brick, const sf::Vector2f& previous_pos) override;

    void onPaddleHit(Paddle& paddle) override;

    void onWallHit() override;

    void onCeilHit() override;

private:
    float   m_angle;
    float   m_velocity;
    int     m_powered;
    // If ball is glued to a sticky paddle
    Paddle* m_glued_to;
    float   m_glued_at;

    ParticleSystem::Emitter m_emitter;

    static int s_instance_count;
};

#endif // BALL_HPP
