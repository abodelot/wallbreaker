#ifndef GUI_OPTIONSBOX_HPP
#define GUI_OPTIONSBOX_HPP

#include "Widget.hpp"

namespace gui
{

template <class T>
class OptionsBox: public Widget
{
public:
	OptionsBox();

	void addItem(const sf::String& label, const T& value);

private:
	struct Item
	{
		BitmapText label;
		T value;
	};

	typedef std::vector<Item> ItemVector;

	ItemVector         m_items;
	sf::RectangleShape m_box;
	int                m_current_index;
};

}

#include "OptionsBox.inl"

#endif // GUI_OPTIONSBOX_HPP
