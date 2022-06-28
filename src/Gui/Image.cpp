#include "Image.hpp"

using namespace gui;

Image::Image():
    m_texture(nullptr)
{
}


void Image::setTexture(const sf::Texture& texture, const sf::IntRect& rect)
{
    float x = static_cast<float>(rect.left);
    float y = static_cast<float>(rect.top);
    float w = static_cast<float>(rect.width);
    float h = static_cast<float>(rect.height);

    m_vertices[0] = sf::Vertex({0, 0}, sf::Color::White, {x, y});
    m_vertices[1] = sf::Vertex({w, 0}, sf::Color::White, {x + w, y});
    m_vertices[2] = sf::Vertex({w, h}, sf::Color::White, {x + w, y + h});
    m_vertices[3] = sf::Vertex({0, h}, sf::Color::White, {x, y + h});

    m_texture = &texture;
    setSize({w, h});
}


void Image::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = m_texture;
    target.draw(m_vertices, 4, sf::Quads, states);
}


void Image::onMouseEnter()
{
    if (hasCallback())
        setCursor(sf::Cursor::Hand);
}


void Image::onMouseLeave()
{
    if (hasCallback())
        setCursor(sf::Cursor::Arrow);
}


void Image::onMouseReleased(const sf::Vector2f& mouse)
{
    if (containsPoint(mouse))
    {
        triggerCallback();
    }
}
