#include "Button.hpp"
#include "Theme.hpp"

#define BUTTON_WIDTH 86

// TODO: padding + borders

Button::Button(const sf::String& string, int id):
	m_text(gui::Theme::getFont()),
	m_type(id)
{
	setText(string);
	m_box.setOutlineThickness(1);
	m_box.setOutlineColor(sf::Color::Cyan);
	m_box.setFillColor(sf::Color::Blue);
}


const sf::Vector2f& Button::getPosition() const
{
	return m_box.getPosition();
}


void Button::setPosition(const sf::Vector2f& position)
{
	m_text.setPosition(position);
	m_box.setPosition(position);
	// center text
	m_text.move((m_box.getSize().x - m_text.getSize().x) / 2, 0);
}


const sf::Vector2f& Button::getSize() const
{
	return m_box.getSize();
}


void Button::setText(const sf::String& string)
{
	m_text.setString(string);
	m_box.setSize(sf::Vector2f(BUTTON_WIDTH, m_text.getSize().y));
	// center text
	m_text.move((m_box.getSize().x - m_text.getSize().x) / 2, 0);
}


bool Button::containsPoint(float x, float y) const
{
	return x > m_box.getPosition().x &&
	       x < m_box.getPosition().x + m_box.getSize().x &&
	       y > m_box.getPosition().y &&
	       y < m_box.getPosition().y + m_box.getSize().y;
}


int Button::getID() const
{
	return m_type;
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
	target.draw(m_box);
	target.draw(m_text);
}
