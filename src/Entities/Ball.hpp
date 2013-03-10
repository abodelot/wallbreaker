#ifndef BALL_HPP
#define BALL_HPP

#include "Entity.hpp"
#include "Core/ParticleSystem.hpp"
class PlayerPad;

class Ball: public Entity, public ParticleSystem::Emitter
{
public:
	Ball();
	~Ball();

	static int getCount() { return m_instance_count; };

	// callbacks ---------------------------------------------------------------

	void onUpdate(float frametime);

	void onBrickHit(Brick& brick);

	void resolveCollision(Entity& entity) const { entity.onCollide(*this); }

	void onCollide(const PlayerPad& pad);

	void onWallHit();

	void onCeilHit();


	float getAngle() const { return m_angle; }
	sf::Vector2f getSpawnPosition() const;

private:
	float m_angle;
	static int m_instance_count;
};

#endif // BALL_HPP
