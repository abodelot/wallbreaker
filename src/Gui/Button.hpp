#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP

#include "Widget.hpp"
#include "BitmapText.hpp"

namespace gui
{

/**
 * A simple press button
 * Triggers an event when clicked
 */
class Button: public Widget
{
public:
	Button(const sf::String& string, int id = -1);

	/**
	 * Set the displayed button label
	 */
	void setString(const sf::String& string);
	const sf::String& getString() const;

	// callbacks ---------------------------------------------------------------

	void onMouseEnter();
	void onMouseLeave();
	void onMousePressed(float x, float y);
	void onMouseReleased(float x, float y);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::RectangleShape m_box;
	BitmapText         m_text;
	bool               m_pressed;
};

}

#endif // GUI_BUTTON_HPP
