#ifndef EASING_HPP
#define EASING_HPP

#include <list>
#include <SFML/Graphics.hpp>

/**
 * Animation manager based on sprite transformations
 */
class Easing
{
public:
	/**
	 * Zoom animation
	 * @param target: sprite to zoom
	 * @param factor: final scale factor
	 */
	static void zoom         (sf::Sprite& target, float factor, float duration = 1.f);
	static void zoomAndRevert(sf::Sprite& target, float factor, float duration = 1.f);

	/**
	 * Move animation
	 * @param target: sprite to move
	 * @param distance: relative movement, final position is current position + distance
	 */
	static void move         (sf::Sprite& target, const sf::Vector2f& distance, float duration = 1.f);
	static void moveAndRevert(sf::Sprite& target, const sf::Vector2f& distance, float duration = 1.f);

	/**
	 * Rotate animation
	 * @param target: sprite to rotate
	 * @param angle: angle of the rotation (degrees), final rotation is current rotation + angle
	 */
	static void rotate         (sf::Sprite& target, float angle, float duration = 1.f);
	static void rotateAndRevert(sf::Sprite& target, float angle, float duration = 1.f);

	/**
	 * Fade-in animation
	 */
	static void fadeIn(sf::Sprite& target, float duration = 1.f);

	/**
	 * Fade-out animation
	 */
	static void fadeOut(sf::Sprite& target, float duration = 1.f);

	/**
	 * Update animations for the current frame
	 */
	static void update(float frametime);

	/**
	 * Stop and delete running animations
	 * All animations will be immiediatly set to their final state
	 */
	static void stopAll();

private:
	enum Effect
	{
		ZOOM,
		ZOOM_REVERT,
		MOVE,
		MOVE_REVERT,
		ROTATE,
		ROTATE_REVERT,
		FADE_IN,
		FADE_OUT,
	};

	struct Object
	{
		struct Zoom
		{
			float final_scale;
			float scale_per_sec;
		};

		struct Move
		{
			float final_x;
			float final_y;
			float x_per_sec;
			float y_per_sec;
		};

		struct Rotate
		{
			float final_angle;
			float angle_per_sec;
		};

		union Data
		{
			Zoom zoom;
			Move move;
			Rotate rotate;
		};

		Object(Effect _effect, sf::Sprite& t, float _duration):
			effect(_effect), target(t), duration(_duration)
		{
			created_at.restart();
		}

		Object(Data& _data, Effect _effect, sf::Sprite& t, float _duration):
			data(_data), effect(_effect), target(t), duration(_duration)
		{
			created_at.restart();
		}

		Data data;
		Effect effect;
		sf::Sprite& target;
		const float duration;
		sf::Clock created_at;
	};

	static void pushObject(Object::Data& data, Effect effect, sf::Sprite& target, float duration);

	typedef std::list<Object> ObjectList;

	static ObjectList m_objects;
};

#endif // EASING_HPP
