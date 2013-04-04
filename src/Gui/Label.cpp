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
	setString(string);
}


void Label::setString(const sf::String& string)
{
	m_text.setString(string);
	m_text.setPosition(Theme::PADDING, Theme::PADDING);
	setSize({Theme::WIDGET_WIDTH, Theme::getBaseLine()});
}


const sf::String& Label::getString() const
{
	return m_text.getString();
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_text, states);
}

