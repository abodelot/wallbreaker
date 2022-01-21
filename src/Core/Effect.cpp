#include "Effect.hpp"


Effect::ObjectList Effect::m_objects;


void Effect::zoom(sf::Sprite& target, float factor, float duration)
{
    Object::Data data;
    data.zoom.final_scale = factor;
    data.zoom.scale_per_sec = (factor - target.getScale().x) / duration;
    pushObject(data, ZOOM, target, duration);
}


void Effect::zoomAndRevert(sf::Sprite& target, float factor, float duration)
{
    Object::Data data;
    data.zoom.final_scale = factor;
    data.zoom.scale_per_sec = (factor - target.getScale().x) / duration;
    pushObject(data, ZOOM_REVERT, target, duration);
}


void Effect::move(sf::Sprite& target, const sf::Vector2f& delta, float duration)
{
    Object::Data data;
    data.move.final_x = target.getPosition().x + delta.x;
    data.move.final_y = target.getPosition().y + delta.y;
    data.move.x_per_sec = delta.x / duration;
    data.move.y_per_sec = delta.y / duration;
    pushObject(data, MOVE, target, duration);
}


void Effect::moveAndRevert(sf::Sprite& target, const sf::Vector2f& delta, float duration)
{
    Object::Data data;
    data.move.final_x = target.getPosition().x + delta.x;
    data.move.final_y = target.getPosition().y + delta.y;
    data.move.x_per_sec = delta.x / duration;
    data.move.y_per_sec = delta.y / duration;
    pushObject(data, MOVE_REVERT, target, duration);
}


void Effect::rotate(sf::Sprite& target, float angle, float duration)
{
    Object::Data data;
    data.rotate.final_angle = target.getRotation() + angle;
    data.rotate.angle_per_sec = angle / duration;
    pushObject(data, ROTATE, target, duration);
}


void Effect::rotateAndRevert(sf::Sprite& target, float angle, float duration)
{
    Object::Data data;
    data.rotate.final_angle = target.getRotation() + angle;
    data.rotate.angle_per_sec = angle / duration;
    pushObject(data, ROTATE_REVERT, target, duration);
}


void Effect::fadeIn(sf::Sprite& target, float duration)
{
    m_objects.push_back(Object(FADE_IN, target, duration));
}


void Effect::fadeOut(sf::Sprite& target, float duration)
{
    m_objects.push_back(Object(FADE_OUT, target, duration));
}


void Effect::update(float frametime)
{
    for (ObjectList::iterator it = m_objects.begin(); it != m_objects.end();)
    {
        float elapsed = it->createdAt.getElapsedTime().asSeconds();
        if (elapsed >= it->duration)
        {
            switch (it->type)
            {
            // Apply the opposite animation
            case ZOOM_REVERT:
            {
                Object::Zoom& zoom = it->data.zoom;
                zoom.scale_per_sec *= -1;
                zoom.final_scale = zoom.final_scale + zoom.scale_per_sec * it->duration;
                it->type = ZOOM;
                it->createdAt.restart();
                continue;
            }
            case MOVE_REVERT:
            {
                Object::Move& move = it->data.move;
                move.x_per_sec *= -1;
                move.y_per_sec *= -1;
                move.final_x = move.final_x + move.x_per_sec * it->duration;
                move.final_y = move.final_y + move.y_per_sec * it->duration;
                it->type = MOVE;
                it->createdAt.restart();
                continue;
            }
            case ROTATE_REVERT:
            {
                Object::Rotate& rotate = it->data.rotate;
                rotate.angle_per_sec *= -1;
                rotate.final_angle = rotate.final_angle + rotate.angle_per_sec * it->duration;
                it->type = ROTATE;
                it->createdAt.restart();
                continue;
            }
            // When effect ends, make sure it reaches the actual final value
            case ZOOM:
                it->target.setScale(it->data.zoom.final_scale, it->data.zoom.final_scale);
                break;
            case MOVE:
                it->target.setPosition(it->data.move.final_x, it->data.move.final_y);
                break;
            case ROTATE:
                it->target.setRotation(it->data.rotate.final_angle);
                break;
            case FADE_IN:
                it->target.setColor({255, 255, 255, 255});
                break;
            case FADE_OUT:
                it->target.setColor({255, 255, 255, 0});
                break;
            }
            it = m_objects.erase(it);
        }
        else
        {
            switch (it->type)
            {
            case ZOOM:
            case ZOOM_REVERT:
                it->target.setScale(
                    it->target.getScale().x + frametime * it->data.zoom.scale_per_sec,
                    it->target.getScale().y + frametime * it->data.zoom.scale_per_sec);
                break;

            case MOVE:
            case MOVE_REVERT:
                it->target.move(
                    frametime * it->data.move.x_per_sec, frametime * it->data.move.y_per_sec);
                break;

            case ROTATE:
            case ROTATE_REVERT:
                it->target.rotate(frametime * it->data.rotate.angle_per_sec);
                break;

            case FADE_IN:
            {
                sf::Uint8 delta = elapsed * 255 / it->duration;
                it->target.setColor(sf::Color(255, 255, 255, delta));
                break;
            }
            case FADE_OUT:
            {
                sf::Uint8 delta = elapsed * 255 / it->duration;
                it->target.setColor(sf::Color(255, 255, 255, 255 - delta));
                break;
            }
            }
            ++it;
        }
    }
}


void Effect::stopAll()
{
    m_objects.clear();
}


void Effect::pushObject(Object::Data& data, Type type, sf::Sprite& target, float duration)
{
    m_objects.push_back(Object(data, type, target, duration));
}
