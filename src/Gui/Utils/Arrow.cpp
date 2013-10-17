#include "Arrow.hpp"

using namespace gui;

Arrow::Arrow()
{
}


Arrow::Arrow(float size, Direction direction)
{
	updateGeometry(0, 0, size, direction);
}


void Arrow::build(float size, Direction direction)
{
	updateGeometry(0, 0, size, direction);
}


void Arrow::setColor(const sf::Color& color)
{
	for (int i = 0; i < 3; ++i)
		m_triangle[i].color = color;
}


void Arrow::setPosition(float x, float y)
{
	float size = std::max(std::abs(m_triangle[0].position.x - m_triangle[1].position.x),
	                      std::abs(m_triangle[1].position.y - m_triangle[1].position.y));
	updateGeometry(x, y, size, m_direction);
}


void Arrow::move(float dx, float dy)
{
	for (int i = 0; i < 3; ++i)
	{
		m_triangle[i].position.x += dx;
		m_triangle[i].position.y += dy;
	}
}


sf::Vector2f Arrow::getSize() const
{
	float size = std::max(std::abs(m_triangle[0].position.x - m_triangle[1].position.x),
	                      std::abs(m_triangle[1].position.y - m_triangle[1].position.y));
	return sf::Vector2f(size, size);
}


void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_triangle, 3, sf::Triangles, states);
}


void Arrow::updateGeometry(float x, float y, float size, Direction direction)
{
	switch (direction)
	{
		// Points are defined clock-wise
		case Top:
			m_triangle[0].position = {x + size / 2, y};
			m_triangle[1].position = {x + size, y + size};
			m_triangle[2].position = {x, y + size};
			break;
		case Bottom:
			m_triangle[0].position = {x, y};
			m_triangle[1].position = {x + size, y};
			m_triangle[2].position = {x + size / 2, y + size};
			break;
		case Left:
			m_triangle[0].position = {x, y + size / 2};
			m_triangle[1].position = {x + size, y};
			m_triangle[2].position = {x + size, y + size};
			break;
		case Right:
			m_triangle[0].position = {x, y};
			m_triangle[1].position = {x + size, y + size / 2};
			m_triangle[2].position = {x, y + size};
			break;
	}
	m_direction = direction;
}
