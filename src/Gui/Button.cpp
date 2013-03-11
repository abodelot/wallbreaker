#include "Button.hpp"
#include "Theme.hpp"

#define BUTTON_WIDTH 86

// TODO: padding + borders

Button::Button(const sf::String& string, int id):
	m_text(gui::Theme::getFont()),
	m_id(id)
{
	setText(string);
	m_box.setOutlineThickness(1);
	m_box.setOutlineColor(sf::Color::Cyan);
	m_box.setFillColor(sf::Color::Blue);
}


const sf::Vector2f& Button::getSize() const
{
	return m_box.getSize();
}


void Button::setText(const sf::String& string)
{
	m_text.setString(string);
	m_box.setSize(sf::Vector2f(BUTTON_WIDTH, m_text.getSize().y));
	// Center text
	m_text.move((m_box.getSize().x - m_text.getSize().x) / 2, 0);
}


bool Button::containsPoint(const sf::Vector2f& point) const
{
	return point.x > 0 && point.x < m_box.getSize().x &&
	       point.y > 0 && point.y < m_box.getSize().y;
}


int Button::getID() const
{
	return m_id;
}


void Button::onMouseEnter()
{
	m_box.setFillColor(sf::Color(64, 128, 255));
}


void Button::onMouseLeave()
{
	m_box.setFillColor(sf::Color::Blue);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);
	target.draw(m_text, states);
}
