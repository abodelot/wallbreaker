#include "Widget.hpp"

using namespace gui;

Widget::Widget(int id):
	m_id(id)
{
}


int Widget::getID() const
{
	return m_id;
}


const sf::Vector2f& Widget::getSize() const
{
	return m_size;
}


void Widget::setSize(const sf::Vector2f& size)
{
	m_size = size;
}


bool Widget::containsPoint(const sf::Vector2f& point) const
{
	return point.x > 0 && point.x < m_size.x &&
	       point.y > 0 && point.y < m_size.y;
}

// callbacks -------------------------------------------------------------------

void Widget::onMouseEnter() {}
void Widget::onMouseLeave() {}
void Widget::onMousePressed(float, float) {}
void Widget::onMouseReleased(float, float) {}
