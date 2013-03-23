#include "Theme.hpp"

#define PADDING 1

namespace gui
{

template <class T>
OptionsBox<T>::OptionsBox():
	m_current_index(-1),
	m_text(Theme::getFont())
{
	float size = Theme::getFont().getGlyphHeight();

	// Build visual components
	m_box.setSize({Theme::WIDGET_WIDTH, size + PADDING * 2});
	m_box.setOutlineThickness(1);
	m_box.setOutlineColor(Theme::BORDER_COLOR);
	m_box.setFillColor(Theme::BG_COLOR);

	m_arrow_left.build(size - PADDING, Arrow::Left);
	m_arrow_left.move(PADDING, PADDING);
	m_arrow_right.build(size - PADDING, Arrow::Right);
	m_arrow_right.move(Theme::WIDGET_WIDTH - PADDING - size, PADDING);

	m_text.move(size, PADDING);

	// Widget local bounds
	setSize(m_box.getSize());
}

template <class T>
void OptionsBox<T>::addItem(const sf::String& label, const T& value)
{
	m_items.push_back(Item(label, value));

	// First inserted item is default selection
	if (m_current_index == -1)
		selectItem(0);
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
}

template <class T>
void OptionsBox<T>::onMousePressed(float x, float y)
{

}

template <class T>
void OptionsBox<T>::onMouseReleased(float x, float y)
{
	float size = Theme::getFont().getGlyphHeight();
	// Select previous item
	if (x < size)
		selectItem(m_current_index == 0 ? m_items.size() - 1 : m_current_index - 1);

	// Select next item
	else if (x > (Theme::WIDGET_WIDTH - size))
		selectItem(m_current_index == (m_items.size() - 1) ? 0 : m_current_index + 1);
}

template <class T>
OptionsBox<T>::Item::Item(const sf::String& string, const T& val):
	label(string),
	value(val)
{
}

}
