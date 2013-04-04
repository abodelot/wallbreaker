#include "Theme.hpp"

namespace gui
{

template <class T>
OptionsBox<T>::OptionsBox():
	m_current_index(-1),
	m_box(BitmapText(Theme::getFont()))
{
	// Build visual components
	m_box.setSize(Theme::WIDGET_WIDTH, Theme::getBaseLine());

	// Pack left arrow
	float arrow_size = Theme::getFont().getGlyphHeight() - 4;
	m_arrow_left.item().set(arrow_size, Arrow::Left);
	m_arrow_left.setSize(Theme::getBaseLine(), Theme::getBaseLine());

	// Pack right arrow
	m_arrow_right.item().set(arrow_size, Arrow::Right);
	m_arrow_right.setSize(Theme::getBaseLine(), Theme::getBaseLine());
	m_arrow_right.move(Theme::WIDGET_WIDTH - Theme::getBaseLine(), 0);

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
		m_box.item().setString(m_items[item_index].label);
		m_box.adjustItem();
	}
}


template <class T>
const T& OptionsBox<T>::getSelectedValue() const
{
	return m_items[m_current_index].value;
}


template <class T>
size_t OptionsBox<T>::getSelectedIndex() const
{
	return m_current_index;
}


template <class T>
void OptionsBox<T>::selectNext()
{
	if (m_items.size() > 1)
	{
		// Get next item index
		selectItem(m_current_index == (m_items.size() - 1) ? 0 : m_current_index + 1);
		triggerCallback();
	}
}


template <class T>
void OptionsBox<T>::selectPrevious()
{
	if (m_items.size() > 1)
	{
		// Get previous item index
		selectItem(m_current_index == 0 ? m_items.size() - 1 : m_current_index - 1);
		triggerCallback();
	}
}


template <class T>
void OptionsBox<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);
	target.draw(m_arrow_left, states);
	target.draw(m_arrow_right, states);
}

// callbacks -------------------------------------------------------------------

template <class T>
void OptionsBox<T>::onStateChanged(State state)
{
	if (state == StateDefault || state == StateFocused)
	{
		m_arrow_left.applyState(state);
		m_arrow_right.applyState(state);
		m_box.applyState(state);
	}
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
		selectPrevious();
		triggerCallback();
	}
	else if (m_arrow_right.containsPoint(x, y))
	{
		selectNext();
		m_arrow_right.release();
	}
}


template <class T>
void OptionsBox<T>::onKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Left)
	{
		selectPrevious();
		m_arrow_left.press();
	}
	else if (key == sf::Keyboard::Right)
	{
		selectNext();
		m_arrow_right.press();
	}
}


template <class T>
void OptionsBox<T>::onKeyReleased(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Left)
	{
		m_arrow_left.release();
	}
	else if (key == sf::Keyboard::Right)
	{
		m_arrow_right.release();
	}
}


template <class T>
OptionsBox<T>::Item::Item(const sf::String& string, const T& val):
	label(string),
	value(val)
{
}

}
