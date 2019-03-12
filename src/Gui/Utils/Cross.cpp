#include <cmath>
#include "Cross.hpp"

using namespace gui;

Cross::Cross()
{
}


void Cross::setPosition(float x, float y)
{
    float size = m_vertices[1].position.x - m_vertices[3].position.x;
    updateGeometry(x, y, size);
}


void Cross::move(float dx, float dy)
{
    for (int i = 0; i < 8; ++i)
    {
        m_vertices[i].position.x += dx;
        m_vertices[i].position.y += dy;
    }
}


void Cross::setSize(float size)
{
    sf::Vector2f position(m_vertices[3].position.x, m_vertices[0].position.y);
    updateGeometry(position.x, position.y, size);
}


sf::Vector2f Cross::getSize() const
{
    return sf::Vector2f(m_vertices[1].position.x - m_vertices[3].position.x,
                        m_vertices[2].position.y - m_vertices[0].position.y);
}


void Cross::setColor(const sf::Color& color)
{
    for (int i = 0; i < 8; ++i)
        m_vertices[i].color = color;
}


void Cross::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vertices, 8, sf::Quads, states);
}


void Cross::updateGeometry(float x, float y, float size)
{
    float offset = std::sqrt(2);
    // top-left to bottom-right
    m_vertices[0].position = {x + offset,        y};
    m_vertices[1].position = {x + size,          y + size - offset};
    m_vertices[2].position = {x + size - offset, y + size};
    m_vertices[3].position = {x,                 y + offset};

    // top-right to bottom-left
    m_vertices[4].position = {x + size - offset, y};
    m_vertices[5].position = {x + size,          y + offset};
    m_vertices[6].position = {x + offset,        y + size};
    m_vertices[7].position = {x,                 y + size - offset};
}
