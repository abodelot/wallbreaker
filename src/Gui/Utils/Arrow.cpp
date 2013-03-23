#include "Arrow.hpp"

using namespace gui;

Arrow::Arrow()
{
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


void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_triangle, 3, sf::Triangles, states);
}
