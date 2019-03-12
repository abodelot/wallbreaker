#ifndef GUI_CROSS_HPP
#define GUI_CROSS_HPP

#include <SFML/Graphics.hpp>

namespace gui
{

/**
 * An X-shapped crosss
 */
class Cross: public sf::Drawable
{
public:
    Cross();

    void setPosition(float x, float y);
    void move(float dx, float dy);

    void setSize(float size);
    sf::Vector2f getSize() const;

    void setColor(const sf::Color& color);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateGeometry(float x, float y, float size);

    sf::Vertex m_vertices[8];
};

}

#endif // GUI_CROSS_HPP
