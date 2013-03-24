#include "Theme.hpp"

#define PADDING 1

namespace gui
{

template <class T>
OptionsBox<T>::OptionsBox():
	m_current_index(-1),
	m_text(Theme::getFont())
{
	// Build visual components
	m_box.setSize({Theme::WIDGET_WIDTH, Theme::getBaseLine()});
	m_box.setOutlineThickness(1);
	m_box.setOutlineColor(Theme::BORDER_COLOR);
	m_box.setFillColor(Theme::BG_COLOR);

	float size = Theme::getFont().getGlyphHeight() - 4;
	m_arrow_left.m_item.build(size, Arrow::Left);
	m_arrow_left.pack(Theme::getBaseLine(), Theme::getBaseLine());

	m_arrow_right.m_item.build(size, Arrow::Right);
	m_arrow_right.pack(Theme::getBaseLine(), Theme::getBaseLine());
	m_arrow_right.move(Theme::WIDGET_WIDTH - Theme::getBaseLine(), 0);

	m_text.move(size, PADDING);

	// Widget local bounds
	setSize(m_box.getSize());
}

template <class T>
void OptionsBox<T>::addItem(const sf::String& label, const T& value, bool select)
{
	m_items.push_back(Item(label, value));

	// First inserted item is default selection
	if (m_items.size() == 1)
		selectItem(0);
	else if (select)
		selectItem(m_items.size() - 1);
}

template <class T>
void OptionsBox<T>::selectItem(size_t item_index)
{
	if (item_index < m_items.size())
	{
		m_current_index = item_index;
		m_text.setString(m_items[item_index].label);
		// Center text
		m_text.setPosition((m_box.getSize().x - m_text.getSize().x) / 2, PADDING);
	}
}

template <class T>
const T& OptionsBox<T>::getSelectedValue() const
{
	return m_items[m_current_index].value;
}

template <class T>
void OptionsBox<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);
	target.draw(m_arrow_left, states);
	target.draw(m_arrow_right, states);
	target.draw(m_text, states);
}

// callbacks -------------------------------------------------------------------

template <class T>
void OptionsBox<T>::onMouseEnter()
{
}

template <class T>
void OptionsBox<T>::onMouseLeave()
{
	m_arrow_left.release();
	m_arrow_right.release();
}

template <class T>
void OptionsBox<T>::onMouseMoved(float x, float y)
{
	if (m_arrow_left.containsPoint(x, y))
		m_arrow_left.prelight();
	else
		m_arrow_left.release();

	if (m_arrow_right.containsPoint(x, y))
		m_arrow_right.prelight();
	else
		m_arrow_right.release();
}


template <class T>
void OptionsBox<T>::onMousePressed(float x, float y)
{
	if (m_arrow_left.containsPoint(x, y))
		m_arrow_left.press();

	else if (m_arrow_right.containsPoint(x, y))
		m_arrow_right.press();
}

template <class T>
void OptionsBox<T>::onMouseReleased(float x, float y)
{
	if (m_arrow_left.containsPoint(x, y))
	{
		// Select previous item
		selectItem(m_current_index == 0 ? m_items.size() - 1 : m_current_index - 1);
		m_arrow_left.release();
		triggerCallback();
	}
	else if (m_arrow_right.containsPoint(x, y))
	{
		// Select next item
		selectItem(m_current_index == (m_items.size() - 1) ? 0 : m_current_index + 1);
		m_arrow_right.release();
		triggerCallback();
	}
}

template <class T>
OptionsBox<T>::Item::Item(const sf::String& string, const T& val):
	label(string),
	value(val)
{
}

}
