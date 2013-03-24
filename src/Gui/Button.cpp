#include "Button.hpp"
#include "Theme.hpp"

#define V_PADDING 1

using namespace gui;

Button::Button(const sf::String& string, int id):
	Widget(id),
	m_text(Theme::getFont()),
	m_pressed(false)
{
	setString(string);
	m_box.setOutlineThickness(1);
	m_box.setOutlineColor(Theme::BORDER_COLOR);
	m_box.setFillColor(Theme::BG_COLOR);
}


void Button::setString(const sf::String& string)
{
	m_text.setString(string);
	m_box.setSize({Theme::WIDGET_WIDTH, m_text.getSize().y + V_PADDING * 2});
	setSize(m_box.getSize());

	// Center text
	m_text.setPosition((m_box.getSize().x - m_text.getSize().x) / 2, V_PADDING);
}


const sf::String& Button::getString() const
{
	return m_text.getString();
}


void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);
	target.draw(m_text, states);
}

// callbacks -------------------------------------------------------------------

void Button::onMouseEnter()
{
	m_box.setFillColor(Theme::BG_COLOR_HOVER);
}


void Button::onMouseLeave()
{
	m_box.setFillColor(Theme::BG_COLOR);
	if (m_pressed)
	{
		m_text.move(0, -1);
		m_pressed = false;
	}
}


void Button::onMousePressed(float, float)
{
	m_box.setFillColor(Theme::BG_COLOR_PRESSED);
	m_text.move(0, 1);
	m_pressed = true;
}


void Button::onMouseReleased(float x, float y)
{
	m_box.setFillColor(Theme::BG_COLOR_HOVER);
	if (m_pressed)
	{
		m_text.move(0, -1);
		m_pressed = false;
		triggerCallback();
	}
}
