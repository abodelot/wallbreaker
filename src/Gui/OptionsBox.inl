#include "Theme.hpp"

namespace gui
{

template <class T>
OptionsBox<T>::OptionsBox():
    m_current_index(-1),
    m_box(BitmapText(Theme::getFont()))
{
    // Build visual components
    m_box.setSize(Theme::WIDGET_WIDTH, Theme::getBaseLine());
    m_box.item().setColor(Theme::TEXT_COLOR);

    // Pack left arrow
    float arrow_size = Theme::getFont().getGlyphHeight() - Theme::PADDING * 2;
    m_arrow_left.item().build(arrow_size, Arrow::Left);
    m_arrow_left.item().setColor(Theme::TEXT_COLOR);
    m_arrow_left.setSize(Theme::getBaseLine(), Theme::getBaseLine());

    // Pack right arrow
    m_arrow_right.item().build(arrow_size, Arrow::Right);
    m_arrow_right.item().setColor(Theme::TEXT_COLOR);
    m_arrow_right.setSize(Theme::getBaseLine(), Theme::getBaseLine());
    m_arrow_right.move(Theme::WIDGET_WIDTH - Theme::getBaseLine(), 0);

    // Widget local bounds
    setSize(m_box.getSize());
}


template <class T>
void OptionsBox<T>::addItem(const sf::String& label, const T& value, bool select)
{
    m_items.push_back(Item(label, value));

    // First inserted item is default selection
    if (m_items.size() == 1)
        selectItem(0);
    else if (select)
        selectItem(m_items.size() - 1);
}


template <class T>
void OptionsBox<T>::selectItem(size_t item_index)
{
    if (item_index < m_items.size())
    {
        m_current_index = item_index;
        m_box.item().setString(m_items[item_index].label);
        m_box.adjustItem();
    }
}


template <class T>
const T& OptionsBox<T>::getSelectedValue() const
{
    return m_items[m_current_index].value;
}


template <class T>
size_t OptionsBox<T>::getSelectedIndex() const
{
    return m_current_index;
}


template <class T>
void OptionsBox<T>::selectNext()
{
    if (m_items.size() > 1)
    {
        // Get next item index
        selectItem(m_current_index == (m_items.size() - 1) ? 0 : m_current_index + 1);
        triggerCallback();
    }
}


template <class T>
void OptionsBox<T>::selectPrevious()
{
    if (m_items.size() > 1)
    {
        // Get previous item index
        selectItem(m_current_index == 0 ? m_items.size() - 1 : m_current_index - 1);
        triggerCallback();
    }
}


template <class T>
void OptionsBox<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    transformStates(states);
    target.draw(m_box, states);
    target.draw(m_arrow_left, states);
    target.draw(m_arrow_right, states);
}


template <class T>
void OptionsBox<T>:: updateArrow(Box<Arrow>& arrow, const sf::Vector2f& pos)
{
    if (arrow.containsPoint(pos))
    {
        if (isFocused() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            arrow.press();
        else
            arrow.prelight();
    }
    else
    {
        arrow.applyState(isFocused() ? StateFocused : StateDefault);
    }
}


// callbacks -------------------------------------------------------------------

template <class T>
void OptionsBox<T>::onStateChanged(State state)
{
    // Hovered state is handled in the onMouseMoved callback
    if (state == StateDefault || state == StateFocused)
    {
        m_arrow_left.applyState(state);
        m_arrow_right.applyState(state);
        m_box.applyState(state);
    }
}


template <class T>
void OptionsBox<T>::onMouseMoved(const sf::Vector2f& pos)
{
    updateArrow(m_arrow_left, pos);
    updateArrow(m_arrow_right, pos);
}


template <class T>
void OptionsBox<T>::onMousePressed(const sf::Vector2f& pos)
{
    if (m_arrow_left.containsPoint(pos))
        m_arrow_left.press();

    else if (m_arrow_right.containsPoint(pos))
        m_arrow_right.press();
}


template <class T>
void OptionsBox<T>::onMouseReleased(const sf::Vector2f& pos)
{
    if (m_arrow_left.containsPoint(pos))
    {
        selectPrevious();
        m_arrow_left.release();
    }
    else if (m_arrow_right.containsPoint(pos))
    {
        selectNext();
        m_arrow_right.release();
    }
}


template <class T>
void OptionsBox<T>::onKeyPressed(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Left)
    {
        selectPrevious();
        m_arrow_left.press();
    }
    else if (key == sf::Keyboard::Right)
    {
        selectNext();
        m_arrow_right.press();
    }
}


template <class T>
void OptionsBox<T>::onKeyReleased(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Left)
    {
        m_arrow_left.release();
    }
    else if (key == sf::Keyboard::Right)
    {
        m_arrow_right.release();
    }
}


template <class T>
OptionsBox<T>::Item::Item(const sf::String& string, const T& val):
    label(string),
    value(val)
{
}

}
