#ifndef GUI_IMAGE_HPP
#define GUI_IMAGE_HPP

#include "Widget.hpp"

namespace gui
{

/**
 * Widget for displaying a sf::Texture
 * Texture must be kept alive during widget lifespan
 */
class Image: public Widget
{
public:
    Image();

    void setTexture(const sf::Texture& tex, const sf::IntRect& rect);

protected:
    void onMouseEnter() override;
    void onMouseLeave() override;
    void onMouseReleased(const sf::Vector2f& mouse) override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    const sf::Texture* m_texture;
    sf::Vertex m_vertices[4];
};

}

#endif // GUI_IMAGE_HPP
