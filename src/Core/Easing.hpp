#ifndef EASING_HPP
#define EASING_HPP

#include <list>
#include <SFML/Graphics.hpp>

class Easing
{
public:
	static void scale(sf::Sprite& target, float from, float to, float duration = 1.f);
	static void scaleAndReset(sf::Sprite& target, float from, float to, float duration = 1.f);

	static void move(sf::Sprite& target, const sf::Vector2f& pos, float duration = 1.f);
	static void moveAndReset(sf::Sprite& target, const sf::Vector2f& pos, float duration = 1.f);

	static void rotate(sf::Sprite& target, float to_angle, float duration = 1.f);

	static void fadeIn(sf::Sprite& target, float duration = 1.f);
	static void fadeOut(sf::Sprite& target, float duration = 1.f);


	static void update(float frametime);

	static void stopAll();

private:
	enum Effect
	{
		SCALE,
		SCALE_AND_RESET,
		MOVE,
		MOVE_AND_RESET,
		FADE_IN,
		FADE_OUT,
		ROTATE
	};

	struct Vector2
	{
		float x;
		float y;
	};

	struct Object
	{
		union Data
		{
			struct Scale
			{
				float from;
				float to;
			};
			Scale scale;

			struct Move
			{
				float from_x;
				float from_y;
				float to_x;
				float to_y;
				float speed_x;
				float speed_y;
			};
			Move move;

			struct Rotate
			{
				float angle;
			};
			Rotate rotate;
		};

		Object(Effect _effect, sf::Sprite& _target, float _duration):
			effect(_effect), target(&_target), duration(_duration)
		{
			created_at.restart();
		}

		Object(Data& _data, Effect _effect, sf::Sprite& _target, float _duration):
			data(_data), effect(_effect), target(&_target), duration(_duration)
		{
			created_at.restart();
		}



		Data data;
		Effect effect;
		sf::Sprite*  target;
		float duration;
		sf::Clock created_at;
	};

	static void pushObject(Object::Data& data, Effect effect, sf::Sprite& target, float duration);

	typedef std::list<Object> ObjectList;

	static ObjectList m_objects;
};

#endif // EASING_HPP
