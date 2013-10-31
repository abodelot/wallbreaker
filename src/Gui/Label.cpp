#include "Label.hpp"
#include "Theme.hpp"

using namespace gui;

Label::Label():
	m_text(Theme::getFont())
{
	setSelectable(false);
}


Label::Label(const sf::String& string):
	m_text(Theme::getFont())
{
	setSelectable(false);
	setText(string);
}


void Label::setText(const sf::String& string)
{
	m_text.setString(string);
	m_text.setPosition(Theme::PADDING, Theme::PADDING);
	setSize({Theme::WIDGET_WIDTH, Theme::getBaseLine()});
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


void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_text, states);
}

