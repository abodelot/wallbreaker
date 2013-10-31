#include "Widget.hpp"
#include "Menu.hpp"

using namespace gui;

Widget::Widget(int id):
	m_parent(NULL),
	m_state(StateDefault),
	m_id(id),
	m_selectable(true)
{
}


void Widget::setID(int id)
{
	m_id = id;
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


bool Widget::isSelectable() const
{
	return m_selectable;
}


bool Widget::isFocused() const
{
	return m_state == StateFocused;
}


void Widget::setSelectable(bool selectable)
{
	m_selectable = selectable;
}


void Widget::triggerCallback()
{
	if (m_parent != NULL)
		m_parent->triggerCallback(this);
}

void Widget::setParent(Menu* menu)
{
	m_parent = menu;
}


void Widget::setState(State state)
{
	m_state = state;
	onStateChanged(state);
}


State Widget::getState() const
{
	return m_state;
}

// callbacks -------------------------------------------------------------------

void Widget::onStateChanged(State) {}
void Widget::onMouseMoved(float, float) {}
void Widget::onMousePressed(float, float) {}
void Widget::onMouseReleased(float, float) {}
void Widget::onKeyPressed(sf::Keyboard::Key) {}
void Widget::onKeyReleased(sf::Keyboard::Key) {}
void Widget::onTextEntered(sf::Uint32) {}
