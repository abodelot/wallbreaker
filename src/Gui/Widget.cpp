#include "Widget.hpp"
#include "Menu.hpp"

using namespace gui;

Widget::Widget():
    m_parent(NULL),
    m_previous(NULL),
    m_next(NULL),
    m_state(StateDefault),
    m_id(-1),
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


void Widget::setPosition(const sf::Vector2f& pos)
{
    m_position = pos;
}


void Widget::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}


const sf::Vector2f& Widget::getPosition() const
{
    return m_position;
}


void Widget::setSize(const sf::Vector2f& size)
{
    m_size = size;
}


void Widget::setSize(float width, float height)
{
    m_size.x = width;
    m_size.y = height;
}


const sf::Vector2f& Widget::getSize() const
{
    return m_size;
}


bool Widget::containsPoint(const sf::Vector2f& point) const
{
    return point.x > 0.f && point.x < m_size.x &&
           point.y > 0.f && point.y < m_size.y;
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


void Widget::triggerCallback(const Widget* triggered)
{
    // Notify parent that a widget is triggered
    // If NULL, this is the widget being triggered
    if (m_parent != NULL)
        m_parent->triggerCallback(triggered == NULL ? this : triggered);
}


void Widget::setParent(Layout* parent)
{
    m_parent = parent;
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


void Widget::transformStates(sf::RenderStates& states) const
{
    states.transform *= sf::Transform(
        1, 0, m_position.x,
        0, 1, m_position.y,
        0, 0, 1);
}


// callbacks -------------------------------------------------------------------

void Widget::onStateChanged(State) {}
void Widget::onMouseMoved(const sf::Vector2f&) {}
void Widget::onMousePressed(const sf::Vector2f&) {}
void Widget::onMouseReleased(const sf::Vector2f&) {}
void Widget::onMouseWheelMoved(int) {}
void Widget::onKeyPressed(sf::Keyboard::Key) {}
void Widget::onKeyReleased(sf::Keyboard::Key) {}
void Widget::onTextEntered(sf::Uint32) {}
