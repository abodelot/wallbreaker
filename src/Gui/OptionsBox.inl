#include "Theme.hpp"

namespace gui
{

template <class T>
OptionsBox<T>::OptionsBox():
    m_currentIndex(-1),
    m_box(BitmapText(Theme::font))
{
    sf::Vector2f size(Theme::widgetWidth, Theme::getBaseHeight());

    // Build visual components
    m_box.setSize(
        Theme::widgetWidth - Theme::getBaseHeight() * 2,
        Theme::getBaseHeight()
    );
    m_box.move(Theme::getBaseHeight(), 0);
    m_box.item().setColor(Theme::textColor);

    // Pack left arrow
    const float arrowSize = Theme::font.getGlyphHeight() - 4;
    m_arrowLeft.item().build(arrowSize, Arrow::Left);
    m_arrowLeft.item().setColor(Theme::textColor);
    m_arrowLeft.setSize(Theme::getBaseHeight(), Theme::getBaseHeight());

    // Pack right arrow
    m_arrowRight.item().build(arrowSize, Arrow::Right);
    m_arrowRight.item().setColor(Theme::textColor);
    m_arrowRight.setSize(Theme::getBaseHeight(), Theme::getBaseHeight());
    m_arrowRight.move(Theme::widgetWidth - Theme::getBaseHeight(), 0);

    // Widget local bounds
    setSize(size);
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
        m_currentIndex = item_index;
        m_box.item().setString(m_items[item_index].label);
        m_box.centerItem();
    }
}


template <class T>
const T& OptionsBox<T>::getSelectedValue() const
{
    return m_items[m_currentIndex].value;
}


template <class T>
size_t OptionsBox<T>::getSelectedIndex() const
{
    return m_currentIndex;
}


template <class T>
void OptionsBox<T>::selectNext()
{
    if (m_items.size() > 1)
    {
        // Get next item index
        selectItem(m_currentIndex == (m_items.size() - 1) ? 0 : m_currentIndex + 1);
        triggerCallback();
    }
}


template <class T>
void OptionsBox<T>::selectPrevious()
{
    if (m_items.size() > 1)
    {
        // Get previous item index
        selectItem(m_currentIndex == 0 ? m_items.size() - 1 : m_currentIndex - 1);
        triggerCallback();
    }
}


template <class T>
void OptionsBox<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    transformStates(states);
    target.draw(m_box, states);
    target.draw(m_arrowLeft, states);
    target.draw(m_arrowRight, states);
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
        m_arrowLeft.applyState(state);
        m_arrowRight.applyState(state);
        m_box.applyState(state);
    }
}


template <class T>
void OptionsBox<T>::onMouseMoved(const sf::Vector2f& pos)
{
    updateArrow(m_arrowLeft, pos);
    updateArrow(m_arrowRight, pos);
}


template <class T>
void OptionsBox<T>::onMousePressed(const sf::Vector2f& pos)
{
    if (m_arrowLeft.containsPoint(pos))
    {
        Theme::clickSound.play();
        m_arrowLeft.press();
    }
    else if (m_arrowRight.containsPoint(pos))
    {
        Theme::clickSound.play();
        m_arrowRight.press();
    }
}


template <class T>
void OptionsBox<T>::onMouseReleased(const sf::Vector2f& pos)
{
    if (m_arrowLeft.containsPoint(pos))
    {
        selectPrevious();
        m_arrowLeft.release();
    }
    else if (m_arrowRight.containsPoint(pos))
    {
        selectNext();
        m_arrowRight.release();
    }
}


template <class T>
void OptionsBox<T>::onKeyPressed(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Left)
    {
        Theme::clickSound.play();
        selectPrevious();
        m_arrowLeft.press();
    }
    else if (key == sf::Keyboard::Right)
    {
        Theme::clickSound.play();
        selectNext();
        m_arrowRight.press();
    }
}


template <class T>
void OptionsBox<T>::onKeyReleased(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Left)
    {
        m_arrowLeft.release();
    }
    else if (key == sf::Keyboard::Right)
    {
        m_arrowRight.release();
    }
}


template <class T>
OptionsBox<T>::Item::Item(const sf::String& string, const T& val):
    label(string),
    value(val)
{
}

}
