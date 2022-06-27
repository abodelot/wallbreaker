#include "Label.hpp"
#include "Theme.hpp"

using namespace gui;

Label::Label():
    m_text(Theme::font)
{
    setSelectable(false);
}


Label::Label(const sf::String& string):
    m_text(Theme::font)
{
    setSelectable(false);
    setText(string);
}


void Label::setText(const sf::String& string)
{
    m_text.setString(string);
    m_text.setColor(Theme::textColor);
    m_text.setPosition(Theme::padding, Theme::padding);
    setSize({m_text.getSize().x + Theme::padding * 2,
        m_text.getSize().y + Theme::padding * 2});
}


const sf::String& Label::getText() const
{
    return m_text.getString();
}


void Label::setColor(const sf::Color& color)
{
    m_text.setColor(color);
}


const sf::Color& Label::getColor() const
{
    return m_text.getColor();
}


void Label::setCharacterSize(int size)
{
    m_text.setScale(size, size);
    setSize({m_text.getSize().x + Theme::padding * 2,
        m_text.getSize().y + Theme::padding * 2});
}


void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_text, states);
}
