#include "Button.hpp"
#include "Theme.hpp"


using namespace gui;

Button::Button(const sf::String& string, int id):
	Widget(id),
	m_box(BitmapText(Theme::getFont()))
{
	setString(string);
}


void Button::setString(const sf::String& string)
{
	m_box.item().setString(string);
	m_box.pack(Theme::WIDGET_WIDTH, Theme::getBaseLine());
	m_box.item().setPosition((m_box.getSize().x - m_box.item().getSize().x) / 2, Theme::PADDING);
	setSize(m_box.getSize());
}


const sf::String& Button::getString() const
{
	return m_box.item().getString();
}


void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);
}

// callbacks -------------------------------------------------------------------

void Button::onMouseEnter()
{
	m_box.prelight();
}


void Button::onMouseLeave()
{
	m_box.release();
}


void Button::onMousePressed(float, float)
{
	m_box.press();
}


void Button::onMouseReleased(float x, float y)
{
	m_box.release();
	if (containsPoint({x, y}))
	{
		triggerCallback();
	}
}


