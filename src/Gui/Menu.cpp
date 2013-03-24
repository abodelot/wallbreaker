#include "Menu.hpp"
#include "Widget.hpp"
#include "Button.hpp"
#include "OptionsBox.hpp"

using namespace gui;

Menu::Menu(sf::RenderTarget& window):
	m_window(window),
	m_hover(NULL),
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
						m_hover->onMouseLeave();

					m_hover = widget;
					widget->onMouseEnter();
				}
				else
				{
					// Hover on the same previously hovered widget
					widget->onMouseMoved(mouse.x, mouse.y);
				}
				return -1;
			}
		}
		if (m_hover != NULL)
		{
			m_hover->onMouseLeave();
			m_hover = NULL;
		}
		break;

	case sf::Event::MouseButtonPressed:
		if (m_hover != NULL)
		{
			sf::Vector2f mouse = getMousePosition(event.mouseButton.x, event.mouseButton.y, m_hover);
			if (m_hover->containsPoint(mouse))
			{
				m_hover->onMousePressed(mouse.x, mouse.y);
			}
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


void Menu::addButton(const sf::String& string, int id)
{
	add(new Button(string, id));
}


void Menu::add(Widget* widget)
{
	sf::Vector2f pos;
	if (!m_widgets.empty())
	{
		Widget* last = m_widgets.back();
		pos.x = last->getPosition().x;
		pos.y = last->getPosition().y + last->getSize().y + Theme::MARGIN;
	}
	else
	{
		pos = m_position;
	}
	widget->setParent(this);
	widget->setPosition(pos);
	m_widgets.push_back(widget);
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
