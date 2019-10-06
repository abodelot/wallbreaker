#include "CheckBox.hpp"

using namespace gui;

CheckBox::CheckBox(bool checked):
    Widget()
{
    m_box.setSize(Theme::font.getGlyphHeight(), Theme::font.getGlyphHeight());
    m_box.setBodyColor(sf::Color::White);
    float size = Theme::font.getGlyphHeight() - 4;
    m_box.item().setSize(size);
    m_box.centerItem();
    check(checked);

    setSize(m_box.getSize());
}


bool CheckBox::isChecked() const
{
    return m_checked;
}


void CheckBox::check(bool checked)
{
    m_checked = checked;
    m_box.item().setColor(m_checked ? sf::Color::Black : sf::Color::Transparent);
}


void CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    transformStates(states);
    target.draw(m_box, states);
}

// callbacks -------------------------------------------------------------------

void CheckBox::onStateChanged(State state)
{
    if (state == State::StateFocused)
    {
        m_box.setBodyColor(sf::Color(255, 200, 180));
    }
    else
    {
        m_box.setBodyColor(sf::Color::White);
    }
}


void CheckBox::onMouseReleased(const sf::Vector2f& pos)
{
    if (containsPoint(pos))
    {
        check(!m_checked);
        triggerCallback();
    }
}


void CheckBox::onKeyPressed(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Space)
    {
        check(!m_checked);
        triggerCallback();
    }
}
