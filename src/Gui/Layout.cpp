#include <stdexcept>
#include "Layout.hpp"
#include "Theme.hpp"
#include "Button.hpp"
#include "Label.hpp"

using namespace gui;


Layout::Layout(Type type):
	m_first(NULL),
	m_last(NULL),
	m_type(type),
	m_hover(NULL),
	m_focus(NULL)
{
}


Layout::~Layout()
{
	// Deallocate all widgets
	const Widget* widget = m_first;
	while (widget != NULL)
	{
		const Widget* next = widget->m_next;
		delete widget;
		widget = next;
	}
}


Layout* Layout::addLayout(Type type)
{
	Layout* layout = new Layout(type);
	add(layout);
	return layout;
}


Widget* Layout::add(Widget* widget, int id)
{
	sf::Vector2f pos;
	if (m_last != NULL)
	{
		if (m_type == Layout::Vertical)
			pos.y = m_last->getPosition().y + m_last->getSize().y + Theme::MARGIN;
		else if (m_type == Layout::Horizontal)
			pos.x = m_last->getPosition().x + m_last->getSize().x + Theme::MARGIN;
	}
	widget->setID(id);
	widget->setPosition(pos);
	push(widget);

	return widget;
}


Button* Layout::addButton(const sf::String& string, int id)
{
	Button* button = new Button(string);
	add(button, id);
	return button;
}


Label* Layout::addLabel(const sf::String& string)
{
	Label* label = new Label(string);
	add(label);
	return label;
}


Widget* Layout::addRow(const sf::String& str, Widget* widget, int id)
{
	if (m_type != Layout::Form)
		throw std::logic_error("Layout::Form must me bused for adding a new row");

	sf::Vector2f pos;
	if (m_last != NULL)
		pos.y = getSize().y + Theme::MARGIN;

	// Label on the left side
	Label* label = new gui::Label(str);
	label->setPosition(pos);
	push(label);

	// Widget on the right side
	float width = label->getSize().x + Theme::MARGIN;
	if (width > m_form.label_width)
	{
		m_form.label_width = width;
		size_t i = 0;
		for (Widget* widget = m_first; widget != NULL; widget = widget->m_next)
		{
			// Re-align previous widgets
			if (i % 2)
				widget->setPosition(width, widget->getPosition().y);
			++i;
		}
	}

	widget->setPosition(m_form.label_width, pos.y);
	widget->setID(id);
	push(widget);
	return widget;
}


Widget* Layout::push(Widget* widget)
{
	widget->setParent(this);

	if (m_first == NULL)
	{
		m_first = m_last = widget;
	}
	else
	{
		m_last->m_next = widget;
		widget->m_previous = m_last;
		m_last = widget;
	}
	recomputeSize();

	// Focus first widget
	/*
	if (m_focus == NULL && widget->isSelectable())
		giveFocus(widget);
	*/
	return widget;
}


void Layout::recomputeSize()
{
	float max_width = 0;
	float max_height = 0;
	//float max_visible_width = 0;
	//float max_visible_height = 0;
	for (const Widget* widget = m_first; widget != NULL; widget = widget->m_next)
	{
		float width = widget->getPosition().x + widget->getSize().x;
		if (width > max_width)
			max_width = width;

		float height = widget->getPosition().y + widget->getSize().y;
		if (height > max_height)
			max_height = height;

		/*float visible_width = widget.getX() + widget.getVisibleWidth();
		if (visible_width > max_visible_width)
			max_visible_width = visible_width;

		float visible_height = widget.getY() + widget.getVisibleHeight();
		if (visible_height > max_visible_height)
			max_visible_height = visible_height;*/
	}

	setSize(max_width, max_height);
	//setVisibleSize(max_visible_width, max_visible_height);
}


void Layout::onStateChanged(State state)
{
	if (state == StateDefault)
	{
		if (m_focus != NULL)
		{
			m_focus->setState(StateDefault);
			m_focus = NULL;
		}
	}
}


void Layout::onMouseMoved(float x, float y)
{
	for (Widget* widget = m_first; widget != NULL; widget = widget->m_next)
	{
		// Convert mouse position to widget coordinates system
		sf::Vector2f mouse = sf::Vector2f(x, y) - widget->getPosition();
		if (widget->containsPoint(mouse))
		{
			if (m_hover != widget)
			{
				// A new widget is hovered
				if (m_hover != NULL)
					m_hover->setState(StateDefault);

				m_hover = widget;
				// Don't send Hovered state if widget is already focused
				if (m_hover != m_focus)
				{
					widget->setState(StateHovered);
				}
			}
			else
			{
				// Hover on the same previously hovered widget
				widget->onMouseMoved(mouse.x, mouse.y);
			}
			return;
		}
	}
	// No widget hovered, remove hovered state
	if (m_hover != NULL)
	{
		m_hover->onMouseMoved(x, y);
		m_hover->setState(m_focus == m_hover ? StateFocused : StateDefault);
		m_hover = NULL;
	}
}


void Layout::onMousePressed(float x, float y)
{
	if (m_hover != NULL)
	{
		// Clicked widget takes focus
		if (m_focus != m_hover)
		{
			giveFocus(m_hover);
		}
		sf::Vector2f mouse = sf::Vector2f(x, y) - m_hover->getPosition();
		m_hover->onMousePressed(mouse.x, mouse.y);
	}
	// User didn't click on a widget, remove focus state
	else if (m_focus != NULL)
	{
		m_focus->setState(StateDefault);
		m_focus = NULL;
	}
}


void Layout::onMouseReleased(float x, float y)
{
	if (m_focus != NULL)
	{
		sf::Vector2f mouse = sf::Vector2f(x, y) - m_focus->getPosition();
		if (m_focus->containsPoint(mouse))
		{
			m_focus->onMouseReleased(mouse.x, mouse.y);
		}
	}
}


void Layout::onMouseWheelMoved(int delta)
{
	if (m_focus != NULL)
	{
		m_focus->onMouseWheelMoved(delta);
	}
}


void Layout::onKeyPressed(sf::Keyboard::Key key)
{
	if (key == Theme::NEXT_WIDGET_KEY)
	{
		if (!focusNextWidget())
			// Try to focus first widget if possible
			focusNextWidget();
	}
	else if (key == Theme::PREV_WIDGET_KEY)
	{
		if (!focusPreviousWidget())
			focusPreviousWidget();
	}
	else if (m_focus != NULL)
	{
		m_focus->onKeyPressed(key);
	}
}


void Layout::onKeyReleased(sf::Keyboard::Key key)
{
	if (m_focus != NULL)
	{
		m_focus->onKeyReleased(key);
	}
}


void Layout::onTextEntered(sf::Uint32 unicode)
{
	if (m_focus != NULL)
	{
		m_focus->onTextEntered(unicode);
	}
}


void Layout::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	transformStates(states);

	for (const Widget* widget = m_first; widget != NULL; widget = widget->m_next)
	{
		target.draw(*widget, states);
	}
}


bool Layout::giveFocus(Widget* widget)
{
	if (widget != NULL)
	{
		if (m_focus != NULL)
		{
			m_focus->setState(StateDefault);
			m_focus = NULL;
		}

		if (widget->isSelectable())
		{
			m_focus = widget;
			widget->setState(StateFocused);
			return true;
		}
	}
	return false;
}


bool Layout::focusPreviousWidget()
{
	// If a subframe is already focused
	if (m_focus != NULL && m_focus->toLayout() != NULL)
	{
		if (m_focus->toLayout()->focusPreviousWidget())
			return true;
	}

	Widget* start = m_focus != NULL ? m_focus->m_previous : m_last;
	for (Widget* widget = start; widget != NULL; widget = widget->m_previous)
	{
		if (widget != NULL)
		{
			Layout* container = widget->toLayout();
			if (container != NULL)
			{
				if (container->focusPreviousWidget())
				{
					giveFocus(container);
					return true;
				}
			}
			else if (giveFocus(widget))
			{
				return true;
			}
		}
	}

	if (m_focus != NULL)
		m_focus->setState(StateDefault);
	m_focus = NULL;
	return false;
}


bool Layout::focusNextWidget()
{
	// If a subframe is already focused
	if (m_focus != NULL && m_focus->toLayout() != NULL)
	{
		if (m_focus->toLayout()->focusNextWidget())
			return true;
	}

	Widget* start = m_focus != NULL ? m_focus->m_next : m_first;
	for (Widget* widget = start; widget != NULL; widget = widget->m_next)
	{
		if (widget != NULL)
		{
			Layout* container = widget->toLayout();
			if (container != NULL)
			{
				if (container->focusNextWidget())
				{
					giveFocus(container);
					return true;
				}
			}
			else if (giveFocus(widget))
			{
				return true;
			}
		}
	}

	if (m_focus != NULL)
		m_focus->setState(StateDefault);
	m_focus = NULL;
	return false;
}