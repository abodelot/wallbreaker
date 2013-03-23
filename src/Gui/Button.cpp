#include "Button.hpp"
#include "Theme.hpp"

#define BUTTON_WIDTH 86

using namespace gui;

Button::Button(const sf::String& string, int id):
	Widget(id),
	m_text(gui::Theme::getFont())
{
	setText(string);
	m_box.setOutlineThickness(1);
	m_box.setOutlineColor(Theme::BORDER_COLOR);
	m_box.setFillColor(Theme::BG_COLOR);
}


void Button::setText(const sf::String& string)
{
	m_text.setString(string);
	m_box.setSize({BUTTON_WIDTH, m_text.getSize().y});
	setSize(m_box.getSize());

	// Center text
	m_text.move((m_box.getSize().x - m_text.getSize().x) / 2, 0);
}


void Button::onMouseEnter()
{
	m_box.setFillColor(Theme::BG_COLOR_HOVER);
}


void Button::onMouseLeave()
{
	m_box.setFillColor(Theme::BG_COLOR);
}


void Button::onMousePressed(float, float)
{
	m_box.setFillColor(Theme::BG_COLOR_PRESSED);
}


void Button::onMouseReleased(float x, float y)
{
	m_box.setFillColor(Theme::BG_COLOR_HOVER);
}


void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);
	target.draw(m_text, states);
}
