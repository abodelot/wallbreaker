#ifndef GUI_OPTIONSBOX_HPP
#define GUI_OPTIONSBOX_HPP

#include "Widget.hpp"
#include "BitmapText.hpp"
#include "Utils/Arrow.hpp"
#include "Utils/Box.hpp"

namespace gui
{

/**
 * Widget for selecting one item from a list
 * Triggers an event when selected item changes
 */
template <class T>
class OptionsBox: public Widget
{
public:
	OptionsBox();

	/**
	 * Append a new item in the list
	 * @param label: displayed label when selected
	 * @param value: value associated
	 * @param select: if true, new item becomes the default selection
	 */
	void addItem(const sf::String& label, const T& value, bool select = false);

	/**
	 * Make an item the current one
	 * @param item_index: position of the item in the list
	 */
	void selectItem(size_t item_index);

	/**
	 * Get the value of the selected item
	 * @return associated value
	 */
	const T& getSelectedValue() const;

	// callbacks ---------------------------------------------------------------

	void onMouseEnter();
	void onMouseLeave();
	void onMouseMoved(float x, float y);
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
	size_t             m_current_index;

	// Visual components
	Box<BitmapText>    m_box;
	Box<Arrow>         m_arrow_left;
	Box<Arrow>         m_arrow_right;
};

}

#include "OptionsBox.inl"

#endif // GUI_OPTIONSBOX_HPP
