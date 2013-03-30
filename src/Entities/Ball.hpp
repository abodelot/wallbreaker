#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"
#include "Core/ParticleSystem.hpp"


class Ball: public Entity, public ParticleSystem::Emitter
{
public:
	Ball();
	~Ball();

	static int getCount() { return s_instance_count; };

	// callbacks ---------------------------------------------------------------

	void onUpdate(float frametime);

	void onBrickHit(Brick& brick, const sf::Vector2f& previous_pos);

	void onPaddleHit(Paddle& paddle);

	void onWallHit();

	void onCeilHit();

	sf::Vector2f getSpawnPosition() const;

private:
	float m_angle;
	float m_velocity;
	static int s_instance_count;
};

#endif // BALL_HPP
