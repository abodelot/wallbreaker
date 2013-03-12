#include "Easing.hpp"


Easing::ObjectList Easing::m_objects;


void Easing::scale(sf::Sprite& target, float from, float to, float duration)
{
	Object::Data data;
	data.scale.from = from;
	data.scale.to = to;
	pushObject(data, SCALE, target, duration);
}


void Easing::scaleAndReset(sf::Sprite& target, float from, float to, float duration)
{
	Object::Data data;
	data.scale.from = from;
	data.scale.to = to;
	pushObject(data, SCALE_AND_RESET, target, duration);
}


void Easing::move(sf::Sprite& target, const sf::Vector2f& pos, float duration)
{
	Object::Data data;
	data.move.from_x = target.getPosition().x;
	data.move.from_y = target.getPosition().y;
	data.move.to_x = pos.x;
	data.move.to_y = pos.y;

	data.move.speed_x = (pos.x - data.move.from_x) / duration;
	data.move.speed_y = (pos.y - data.move.from_y) / duration;
	pushObject(data, MOVE, target, duration);
}


void Easing::moveAndReset(sf::Sprite& target, const sf::Vector2f& pos, float duration)
{
	Object::Data data;
	data.move.from_x = target.getPosition().x;
	data.move.from_y = target.getPosition().y;
	data.move.to_x = pos.x;
	data.move.to_y = pos.y;

	data.move.speed_x = (pos.x - data.move.from_x) / duration;
	data.move.speed_y = (pos.y - data.move.from_y) / duration;
	pushObject(data, MOVE, target, duration);
	pushObject(data, MOVE_AND_RESET, target, duration);
}


void Easing::rotate(sf::Sprite& target, float to_angle, float duration)
{
	Object::Data data;
	data.rotate.angle = to_angle;
	pushObject(data, ROTATE, target, duration);
}


void Easing::fadeIn(sf::Sprite& target, float duration)
{
	m_objects.push_back(Object(FADE_IN, target, duration));
}


void Easing::fadeOut(sf::Sprite& target, float duration)
{
	m_objects.push_back(Object(FADE_OUT, target, duration));
}


void Easing::update(float frametime)
{
	for (ObjectList::iterator it = m_objects.begin(); it != m_objects.end();)
	{
		float elapsed = it->created_at.getElapsedTime().asSeconds();
		if (elapsed >= it->duration)
		{

			switch (it->effect)
			{
				case SCALE_AND_RESET:
					Easing::scale(*it->target, it->data.scale.to, it->data.scale.from, it->duration);
					break;
				case MOVE_AND_RESET:
				{
					it->target->setPosition(it->data.move.to_x, it->data.move.to_y);
					sf::Vector2f pos(it->data.move.from_x, it->data.move.from_y);
					Easing::move(*it->target, pos, it->duration);
					break;
				}
				// When effect ends, make sure to stop at the actual value
				case FADE_IN:
					it->target->setColor({255, 255, 255, 255});
					break;
				case FADE_OUT:
					it->target->setColor({255, 255, 255, 0});
					break;
				case ROTATE:
					it->target->setRotation(it->data.rotate.angle);
					break;
				default:
					break;
			}
			it = m_objects.erase(it);
		}
		else
		{
			switch (it->effect)
			{
				case SCALE:
				case SCALE_AND_RESET:
				{
					Object::Data::Scale& s = it->data.scale;
					float new_scale = elapsed * (s.to - s.from) / it->duration + s.from;
					it->target->setScale(new_scale, new_scale);
					break;
				}
				case MOVE:
				case MOVE_AND_RESET:
				{
					Object::Data::Move& move = it->data.move;
					it->target->move(move.speed_x * frametime, move.speed_y * frametime);
					break;
				}
				case FADE_IN:
				{
					sf::Uint8 delta = elapsed * 255 / it->duration;
					it->target->setColor(sf::Color(255, 255, 255, delta));
					break;
				}
				case FADE_OUT:
				{
					sf::Uint8 delta = elapsed * 255 / it->duration;
					it->target->setColor(sf::Color(255, 255, 255, 255 - delta));
					break;
				}
				case ROTATE:
				{
					float delta = elapsed * it->data.rotate.angle / it->duration;
					it->target->setRotation(delta);
					break;
				}
			}
			++it;
		}
	}
}


void Easing::stopAll()
{
	m_objects.clear();
}


void Easing::pushObject(Object::Data& data, Effect effect, sf::Sprite& target, float duration)
{
	m_objects.push_back(Object(data, effect, target, duration));
}
