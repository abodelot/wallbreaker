#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"
#include "Core/ParticleSystem.hpp"


class Ball: public Entity, public ParticleSystem::Emitter
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

	// callbacks ---------------------------------------------------------------

	void onUpdate(float frametime);

	void onBrickHit(Brick& brick, const sf::Vector2f& previous_pos);

	void onPaddleHit(Paddle& paddle);

	void onWallHit();

	void onCeilHit();

	sf::Vector2f getSpawnPosition() const;

private:
	float   m_angle;
	float   m_velocity;
	bool    m_powered;
	// If ball is glued to a sticky paddle
	Paddle* m_glued_to;
	float   m_glued_at;

	static int s_instance_count;
};

#endif // BALL_HPP
