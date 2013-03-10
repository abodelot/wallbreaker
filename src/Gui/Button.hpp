#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP

#include "BitmapText.hpp"

class Button: public sf::Drawable
{
public:
	Button(const sf::String& string, int id = -1);

	const sf::Vector2f& getPosition() const;
	float getX() const { return m_box.getPosition().x; }
	float getY() const { return m_box.getPosition().y; }
	void setPosition(const sf::Vector2f& position);

	const sf::Vector2f& getSize() const;

	void setText(const sf::String& string);

	bool containsPoint(float x, float y) const;

	int getID() const;

	// callbacks
	void onMouseEnter();
	void onMouseLeave();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::RectangleShape m_box;
	BitmapText         m_text;
	int        m_type;
};

#endif // GUI_BUTTON_HPP
