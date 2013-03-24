#include "../Theme.hpp"

namespace gui
{

template <class T>
Box<T>::Box():
	m_pressed(false)
{
	m_background.setFillColor(Theme::BG_COLOR);
	m_background.setOutlineColor(Theme::BORDER_COLOR);
	m_background.setOutlineThickness(1);
}

template <class T>
void Box<T>::move(float dx, float dy)
{
	m_background.move(dx, dy);
	m_item.move(dx, dy);
}

template <class T>
void Box<T>::pack(float width, float height)
{
	m_background.setSize({width, height});

	// center item
	m_item.move((width - m_item.getSize().x) / 2,
				(height - m_item.getSize().y) / 2);

}

template <class T>
void Box<T>::prelight()
{
	if (!m_pressed)
	{
		m_background.setFillColor(Theme::BG_COLOR_HOVER);
	}
}

template <class T>
void Box<T>::press()
{
	if (!m_pressed)
	{
		m_item.move(0, 1);
		m_pressed = true;
		m_background.setFillColor(Theme::BG_COLOR_PRESSED);
	}
}

template <class T>
void Box<T>::release()
{
	if (m_pressed)
	{
		m_item.move(0, -1);
		m_pressed = false;
	}
	m_background.setFillColor(Theme::BG_COLOR);
}

template <class T>
bool Box<T>::containsPoint(float x, float y) const
{
	const sf::Vector2f& pos = m_background.getPosition();
	return x >= pos.x &&
	       x <  pos.x + m_background.getSize().x&&
	       y >= pos.y &&
	       y <  pos.y + m_background.getSize().y;
}

template <class T>
void Box<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background, states);
	target.draw(m_item, states);
}

}
