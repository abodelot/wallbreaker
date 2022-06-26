#include "../Theme.hpp"


namespace gui
{

template <class T>
Box<T>::Box():
    m_pressed(false)
{
    setBodyColor(Theme::backgroundColor);
    setTopLeftBorderColor(Theme::topBorderColor);
    setBottomRightBorderColor(Theme::bottomBorderColor);
}


template <class T>
Box<T>::Box(const T& item):
    m_item(item),
    m_pressed(false)
{
    setBodyColor(Theme::backgroundColor);
    setTopLeftBorderColor(Theme::topBorderColor);
    setBottomRightBorderColor(Theme::bottomBorderColor);
}

// Geometry --------------------------------------------------------------------

template <class T>
const sf::Vector2f& Box<T>::getPosition() const
{
    return m_topLeftBorder[0].position;
}


template <class T>
void Box<T>::setPosition(float x, float y)
{
    const sf::Vector2f delta = sf::Vector2f(x, y) - getPosition();
    for (size_t i = 0; i < 4; ++i)
    {
        m_background[i].position += delta;
        m_topLeftBorder[i].position += delta;
        m_bottomRightBorder[i].position += delta;
    }
    m_item.move(delta.x, delta.y);
}


template <class T>
void Box<T>::setSize(float width, float height)
{
    const sf::Vector2f pos = getPosition();

    // Borders
    m_topLeftBorder[0].position = {0, 0};
    m_topLeftBorder[1].position = {width, 0};
    m_topLeftBorder[2].position = {width, height};
    m_topLeftBorder[3].position = {0, height};

    m_bottomRightBorder[0].position = {Theme::borderSize, Theme::borderSize};
    m_bottomRightBorder[1].position = {width, Theme::borderSize};
    m_bottomRightBorder[2].position = {width, height};
    m_bottomRightBorder[3].position = {Theme::borderSize, height};

    // Body
    const float innerWidth = width - Theme::borderSize;
    const float innerHeight = height - Theme::borderSize;
    m_background[0].position = {Theme::borderSize, Theme::borderSize};
    m_background[1].position = {innerWidth, Theme::borderSize};
    m_background[2].position = {innerWidth, innerHeight};
    m_background[3].position = {Theme::borderSize, innerHeight};

    centerItem();

    // Restore position
    setPosition(pos.x, pos.y);
}


template <class T>
sf::Vector2f Box<T>::getSize() const
{
    // Bottom right corner - top left corner
    return m_topLeftBorder[2].position - m_topLeftBorder[0].position;
}


template <class T>
void Box<T>::centerItem()
{
    sf::Vector2f size = getSize();
    // Center item
    m_item.setPosition(
        m_topLeftBorder[0].position.x + (size.x - m_item.getSize().x) / 2,
        m_topLeftBorder[0].position.y + (size.y - m_item.getSize().y) / 2
    );

    // If item is adjusted while pressed, re-apply the 1px vertical offset
    if (m_pressed)
        m_item.move(0, 1);
}


template <class T>
bool Box<T>::containsPoint(const sf::Vector2f& pos) const
{
    return pos.x >= m_topLeftBorder[0].position.x  // Left
        && pos.x <= m_topLeftBorder[2].position.x  // Right
        && pos.y >= m_topLeftBorder[0].position.y  // Top
        && pos.y <= m_topLeftBorder[2].position.y; // Bottom
}

// Visual properties -----------------------------------------------------------

template <class T>
void Box<T>::prelight()
{
    if (!m_pressed)
    {
        setBodyColor(Theme::hoverColor);
    }
}


template <class T>
void Box<T>::press()
{
    if (!m_pressed)
    {
        m_item.move(0, 1);
        m_pressed = true;
        setBodyColor(Theme::focusColor);
    }
}


template <class T>
void Box<T>::release()
{
    if (m_pressed)
    {
        m_item.move(0, -1);
        m_pressed = false;
    }
}


template <class T>
void Box<T>::applyState(State state)
{
    switch (state)
    {
        case StateHovered:
            prelight();
            break;
        case StateFocused:
            release();
            setBodyColor(Theme::focusColor);
            break;
        case StateDefault:
            release();
            setBodyColor(Theme::backgroundColor);
            break;
        default:
            break;
    }
}


template <class T>
void Box<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_topLeftBorder, 4, sf::Quads, states);
    target.draw(m_bottomRightBorder, 4, sf::Quads, states);
    target.draw(m_background, 4, sf::Quads, states);
    target.draw(m_item, states);
}


template <class T>
void Box<T>::setBodyColor(const sf::Color& color)
{
    for (size_t i = 0; i < 4; ++i)
    {
        m_background[i].color = color;
    }
}


template <class T>
void Box<T>::setTopLeftBorderColor(const sf::Color& color)
{
    for (size_t i = 0; i < 4; ++i)
    {
        m_topLeftBorder[i].color = color;
    }
}


template <class T>
void Box<T>::setBottomRightBorderColor(const sf::Color& color)
{
    for (size_t i = 0; i < 4; ++i)
    {
        m_bottomRightBorder[i].color = color;
    }
}

}
