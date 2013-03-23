#ifndef GUI_OPTIONSBOX_HPP
#define GUI_OPTIONSBOX_HPP

#include "Widget.hpp"
#include "BitmapText.hpp"
#include "Utils/Arrow.hpp"

namespace gui
{

/**
 * Widget for selecting one item from a list
 */
template <class T>
class OptionsBox: public Widget
{
public:
	OptionsBox();

	void addItem(const sf::String& label, const T& value);

	void selectItem(size_t item_index);

	// callbacks ---------------------------------------------------------------

	void onMouseEnter();
	void onMouseLeave();
	void onMousePressed(float x, float y);
	void onMouseReleased(float x, float y);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	struct Item
	{
		Item(const sf::String& string, const T& value);

		sf::String label;
		T value;
	};

	typedef std::vector<Item> ItemVector;

	ItemVector         m_items;
	int                m_current_index;

	// Visual components
	sf::RectangleShape m_box;
	BitmapText         m_text;
	Arrow              m_arrow_left;
	Arrow              m_arrow_right;

};

}

#include "OptionsBox.inl"

#endif // GUI_OPTIONSBOX_HPP
