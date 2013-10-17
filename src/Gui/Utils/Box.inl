#include "../Theme.hpp"


namespace gui
{

template <class T>
Box<T>::Box():
	m_pressed(false)
{
	setBodyColor(Theme::BG_COLOR);
	setBorderColor(Theme::BORDER_COLOR);
}


template <class T>
Box<T>::Box(const T& item):
	m_item(item),
	m_pressed(false)
{
	setBodyColor(Theme::BG_COLOR);
	setBorderColor(Theme::BORDER_COLOR);
}

// Geometry --------------------------------------------------------------------

template <class T>
const sf::Vector2f& Box<T>::getPosition() const
{
	return m_background[0].position;
}


template <class T>
void Box<T>::move(float dx, float dy)
{
	for (size_t i = 0; i < 8; ++i)
		m_background[i].position += sf::Vector2f(dx, dy);

	m_item.move(dx, dy);
}


template <class T>
void Box<T>::setSize(float width, float height)
{
	// Borders
	m_background[0].position = sf::Vector2f(0,     0);
	m_background[1].position = sf::Vector2f(width, 0);
	m_background[2].position = sf::Vector2f(width, height);
	m_background[3].position = sf::Vector2f(0,     height);

	// Body
	float border = Theme::BORDER_SIZE;
	m_background[4].position = sf::Vector2f(border,         border);
	m_background[5].position = sf::Vector2f(width - border, border);
	m_background[6].position = sf::Vector2f(width - border, height - border);
	m_background[7].position = sf::Vector2f(border,         height - border);

	adjustItem();

}


template <class T>
sf::Vector2f Box<T>::getSize() const
{
	// bottom right corner - top left corner
	return m_background[2].position - m_background[0].position;
}


template <class T>
void Box<T>::adjustItem()
{
	sf::Vector2f size = getSize();
	// Center item
	m_item.setPosition(m_background[0].position.x + (size.x - m_item.getSize().x) / 2,
	                   m_background[0].position.y + (size.y - m_item.getSize().y) / 2);

	// If item adjusted while pressed, set the offset back
	if (m_pressed)
		m_item.move(0, 1);
}


template <class T>
bool Box<T>::containsPoint(float x, float y) const
{
	return x >= m_background[0].position.x  // Left
	    && x <= m_background[2].position.x  // Right
	    && y >= m_background[0].position.y  // Top
	    && y <= m_background[2].position.y; // Bottom
}

// Visual properties -----------------------------------------------------------

template <class T>
void Box<T>::prelight()
{
	if (!m_pressed)
	{
		setBodyColor(Theme::BG_COLOR_HOVER);
	}
}


template <class T>
void Box<T>::press()
{
	if (!m_pressed)
	{
		m_item.move(0, 1);
		m_pressed = true;
		setBodyColor(Theme::BG_COLOR_FOCUS);
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
}


template <class T>
void Box<T>::applyState(State state)
{
	switch (state)
	{
        case StateHovered:
			prelight();
			break;
		case StateFocused:
			release();
			setBodyColor(Theme::BG_COLOR_FOCUS);
			break;
		case StateDefault:
			release();
			setBodyColor(Theme::BG_COLOR);
			break;
		default:
			break;
	}
}


template <class T>
void Box<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background, 8, sf::Quads, states);
	target.draw(m_item, states);
}


template <class T>
void Box<T>::setBodyColor(const sf::Color& color)
{
	for (size_t i = 4; i < 8; ++i)
		m_background[i].color = color;
}


template <class T>
void Box<T>::setBorderColor(const sf::Color& color)
{
	for (size_t i = 0; i < 4; ++i)
		m_background[i].color = color;
}

}
