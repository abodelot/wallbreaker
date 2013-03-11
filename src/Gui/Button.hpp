#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP

#include "BitmapText.hpp"

class Button: public sf::Drawable, public sf::Transformable
{
public:
	Button(const sf::String& string, int id = -1);

	float getX() const { return getPosition().x; }
	float getY() const { return getPosition().y; }

	const sf::Vector2f& getSize() const;

	void setText(const sf::String& string);

	bool containsPoint(const sf::Vector2f& point) const;

	int getID() const;

	// callbacks
	void onMouseEnter();
	void onMouseLeave();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::RectangleShape m_box;
	BitmapText         m_text;
	int                m_id;
};

#endif // GUI_BUTTON_HPP
