#include "Widget.hpp"
#include "Menu.hpp"

using namespace gui;

Widget::Widget():
    m_parent(nullptr),
    m_previous(nullptr),
    m_next(nullptr),
    m_state(StateDefault),
    m_selectable(true)
{
    updateTransform();
}


void Widget::setPosition(const sf::Vector2f& pos)
{
    m_position = pos;
    updateTransform();
}


void Widget::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
    updateTransform();
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
    return point.x > 0.f && point.x < m_size.x && point.y > 0.f && point.y < m_size.y;
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


void Widget::setCallback(std::function<void(void)> callback)
{
    m_callback = callback;
}


void Widget::triggerCallback()
{
    if (m_callback)
    {
        m_callback();
    }
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


void Widget::setCursor(sf::Cursor::Type cursorType)
{
    if (m_parent)
        m_parent->setCursor(cursorType);
}


const sf::Transform& Widget::getTransform() const
{
    return m_transform;
}


void Widget::updateTransform()
{
    // Position is the only transformation
    m_transform = sf::Transform(
        1, 0, m_position.x,
        0, 1, m_position.y,
        0, 0, 1);
}
