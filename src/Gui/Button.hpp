#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP

#include "Widget.hpp"
#include "BitmapText.hpp"

namespace gui
{

class Button: public Widget
{
public:
	Button(const sf::String& string, int id = -1);

	void setText(const sf::String& string);


	// callbacks
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
