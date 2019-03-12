#ifndef GUI_ARROW_HPP
#define GUI_ARROW_HPP

#include <SFML/Graphics.hpp>

namespace gui
{

/**
 * A simple arrow made of 3 points
 */
class Arrow: public sf::Drawable
{
public:
    enum Direction { Left, Right, Top, Bottom };

    Arrow();
    Arrow(float size, Direction direction);

    void build(float size, Direction direction);

    void setColor(const sf::Color& color);

    void move(float dx, float dy);
    void setPosition(float x, float y);

    sf::Vector2f getSize() const;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void updateGeometry(float x, float y, float size, Direction direction);

    sf::Vertex m_triangle[3];
    Direction m_direction;
};

}

#endif // GUI_ARROW_HPP
