#include "Menu.hpp"
#include "Widget.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "Theme.hpp"

using namespace gui;

Menu::Menu(sf::RenderTarget& window):
	m_window(window),
	m_hover(NULL),
	m_focus(NULL),
	m_triggered(NULL)
{
}


Menu::~Menu()
{
	for (WidgetVector::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it)
	{
		delete *it;
	}
}


int Menu::onEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::MouseMoved:
		for (WidgetVector::iterator it = m_widgets.begin(); it != m_widgets.end(); ++it)
		{
			Widget* widget = *it;
			// Convert mouse position to widget coordinates system
			sf::Vector2f mouse = getMousePosition(event.mouseMove.x, event.mouseMove.y, widget);
			if (widget->containsPoint(mouse))
			{
				if (m_hover != widget)
				{
					// A new widget is hovered
					if (m_hover != NULL)
						m_hover->onStateChanged(StateDefault);

					m_hover = widget;
					widget->onStateChanged(StateHovered);
				}
				else
				{
					// Hover on the same previously hovered widget
					widget->onMouseMoved(mouse.x, mouse.y);
				}
				return -1;
			}
		}
		// Removed hovered state
		if (m_hover != NULL)
		{
			m_hover->onStateChanged(m_focus == m_hover ? StateFocused : StateDefault);
			m_hover = NULL;
		}
		break;

	case sf::Event::MouseButtonPressed:
		if (m_hover != NULL)
		{
			sf::Vector2f mouse = getMousePosition(event.mouseButton.x, event.mouseButton.y, m_hover);
			if (m_hover->containsPoint(mouse))
			{
				// Clicked widget takes focus
				if (m_focus != m_hover)
				{
					giveFocus(m_hover);
				}
				m_hover->onMousePressed(mouse.x, mouse.y);
			}
		}
		else if (m_focus != NULL)
		{
			// User didn't click on a widget, focus is lost
			m_focus->onStateChanged(StateDefault);
			m_focus = NULL;
		}
		break;

	case sf::Event::MouseButtonReleased:
		if (m_hover != NULL)
		{
			sf::Vector2f mouse = getMousePosition(event.mouseButton.x, event.mouseButton.y, m_hover);
			if (m_hover->containsPoint(mouse))
			{
				m_hover->onMouseReleased(mouse.x, mouse.y);
			}
		}
		break;

	case sf::Event::MouseWheelMoved:
		if (event.mouseWheel.delta)
		{

		}
		break;

	case sf::Event::KeyPressed:
		if (event.key.code == Theme::NEXT_WIDGET_KEY)
		{
			focusNextWidget();
		}
		else if (event.key.code == Theme::PREV_WIDGET_KEY)
		{
			focusPreviousWidget();
		}
		else if (m_focus != NULL)
		{
			m_focus->onKeyPressed(event.key.code);
		}
		break;

	case sf::Event::KeyReleased:
		if (m_focus != NULL)
		{
			m_focus->onKeyReleased(event.key.code);
		}
		break;

	default:
		break;
	}

	if (m_triggered != NULL)
	{
		int id = m_triggered->getID();
		m_triggered = NULL;
		return id;
	}
	return -1;
}


void Menu::setPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}


const sf::Vector2f& Menu::getPosition() const
{
	return m_position;
}


Button* Menu::addButton(const sf::String& string, int id)
{
	Button* button = new Button(string);
	add(button, id);
	return button;
}


Widget* Menu::add(const sf::String& str_label, Widget* widget, int id)
{
	add(widget, id);

	sf::Vector2f pos = widget->getPosition();

	Label* label = new Label(str_label);
	add(label);
	label->setPosition(pos);
	widget->move(label->getSize().x + Theme::MARGIN, 0);
	return widget;
}


Widget* Menu::add(Widget* widget, int id)
{
	sf::Vector2f pos;
	if (!m_widgets.empty())
	{
		Widget* last = m_widgets.back();
		pos.x = last->getPosition().x;
		pos.y = last->getPosition().y + last->getSize().y * last->getScale().y + Theme::MARGIN;
	}
	else
	{
		pos = m_position;
	}
	widget->setID(id);
	widget->setParent(this);
	widget->setPosition(pos);
	m_widgets.push_back(widget);

	// Focus first widget
	if (m_focus == NULL && widget->isSelectable())
		giveFocus(widget);

	return widget;
}


void Menu::triggerCallback(const Widget* widget)
{
	m_triggered = widget;
}


void Menu::draw() const
{
	for (WidgetVector::const_iterator it = m_widgets.begin(); it != m_widgets.end(); ++it)
	{
		const Widget& widget = **it;
		m_window.draw(widget);
	}
}


sf::Vector2f Menu::getMousePosition(int x, int y, const Widget* relative) const
{
	sf::Vector2f mouse = m_window.mapPixelToCoords({x, y});
	if (relative != NULL)
		mouse -= relative->getPosition();
	return mouse;
}


void Menu::focusNextWidget()
{
	if (m_widgets.empty())
		return;

	// Don't search the focused widget if none is focused
	bool found_current = m_focus == NULL;
	Widget* first_selectable = NULL;

	for (size_t i = 0; i < m_widgets.size(); ++i)
	{
		if (m_widgets[i] == m_focus)
		{
			found_current = true;
		}
		else if (m_widgets[i]->isSelectable())
		{
			if (first_selectable == NULL)
				first_selectable = m_widgets[i];

			if (found_current)
			{
				giveFocus(m_widgets[i]);
				return;
			}
		}
	}
	giveFocus(first_selectable);
}


void Menu::focusPreviousWidget()
{
	if (m_widgets.empty())
		return;

	// Don't search the focused widget if none is focused
	bool found_current = m_focus == NULL;
	Widget* last_selectable = NULL;
	for (auto it = m_widgets.rbegin(); it != m_widgets.rend(); ++it)
	{
		if (*it == m_focus)
		{
			found_current = true;
		}
		else if ((**it).isSelectable())
		{
			if (last_selectable == NULL)
			{
				last_selectable = *it;
			}

			if (found_current)
			{
				giveFocus(*it);
				return;
			}
		}
	}
	giveFocus(last_selectable);
}


void Menu::giveFocus(Widget* widget)
{
	if (widget == NULL)
		return;

	if (m_focus != NULL)
		m_focus->onStateChanged(StateDefault);

	m_focus = widget;
	widget->onStateChanged(StateFocused);
}
