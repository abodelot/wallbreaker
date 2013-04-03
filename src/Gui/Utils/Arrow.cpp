#include "Arrow.hpp"
#include <SFML/OpenGL.hpp>

using namespace gui;

Arrow::Arrow()
{
}


Arrow::Arrow(float size, Direction direction)
{
	build(size, direction);
}


void Arrow::build(float size, Direction direction)
{
	switch (direction)
	{
		// Points are defined clock-wise
		case Top:
			m_triangle[0].position = {size / 2, 0};
			m_triangle[1].position = {size, size};
			m_triangle[2].position = {0, size};
			break;
		case Bottom:
			m_triangle[0].position = {0, 0};
			m_triangle[1].position = {size, 0};
			m_triangle[2].position = {size / 2, size};
			break;
		case Left:
			m_triangle[0].position = {0, size / 2};
			m_triangle[1].position = {size, 0};
			m_triangle[2].position = {size, size};
			break;
		case Right:
			m_triangle[0].position = {0, 0};
			m_triangle[1].position = {size, size / 2};
			m_triangle[2].position = {0, size};
			break;
	}
}


void Arrow::setColor(const sf::Color& color)
{
	for (int i = 0; i < 3; ++i)
		m_triangle[i].color = color;
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
	glLineWidth(0);
	target.draw(m_triangle, 3, sf::Triangles, states);
}
