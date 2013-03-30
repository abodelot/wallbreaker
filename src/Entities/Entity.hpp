#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics/Sprite.hpp>

class Ball;
class Paddle;
class Brick;
class Wallbreaker;

/**
 * Base class for game entities
 */
class Entity: public sf::Sprite
{
public:
	Entity();
	virtual ~Entity();

	void setParent(Wallbreaker* parent);
	Wallbreaker* getParent();

	inline bool isAlive() const { return m_alive; }

	inline void kill() { m_alive = false; }

	bool collidesWith(const Entity& other) const;

	// casts -------------------------------------------------------------------

	virtual Ball* toBall();

	// callbacks ---------------------------------------------------------------

	virtual void onUpdate(float frametime) = 0;

	virtual void onBrickHit(Brick& brick, const sf::Vector2f& previous_pos) {};
	virtual void onPaddleHit(Paddle& paddle) {}

	virtual void onCeilHit() {}
	virtual void onWallHit() {}

	// position helpers --------------------------------------------------------

	inline float getX() const { return getPosition().x; }
	inline float getY() const { return getPosition().y; }
	inline void setX(float x) { setPosition(x, getPosition().y); }
	inline void setY(float y) { setPosition(getPosition().x, y); }

	// size helpers ------------------------------------------------------------

	float getWidth() const;
	float getHeight() const;
	sf::IntRect getCollisionRect() const;

private:
	Wallbreaker* m_parent;
	bool         m_alive;
};

#endif // ENTITY_HPP
