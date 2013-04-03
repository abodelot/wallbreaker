#ifndef GUI_BOX_HPP
#define GUI_BOX_HPP

#include <SFML/Graphics.hpp>

namespace gui
{

/**
 * Utility class used by widgets for building visual component
 */
template <class T>
class Box: public sf::Drawable
{
public:
	Box();
	Box(const T& item);

	void pack(float width, float height);
	void move(float dx, float dy);

	/**
	 * @return true if point is inside the box limits
	 */
	bool containsPoint(float x, float y) const;

	/**
	 * Get box dimensions
	 */
	sf::Vector2f getSize() const;

	/**
	 * Apply "hovered" effect
	 */
	void prelight();

	/**
	 * Apply "pressed" effect
	 */
	void press();

	/**
	 * Remove applied effect
	 */
	void release();





	/**
	 * Access to the encapsulated object
	 */
	inline       T& item()       { return m_item; }
	inline const T& item() const { return m_item; }

	void setBodyColor(const sf::Color& color);
	void setBorderColor(const sf::Color& color);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	T          m_item;
	bool       m_pressed;
	sf::Vertex m_background[8]; // 0-4 for body, 4-8 for borders
};

}

#include "Box.inl"

#endif // GUI_BOX_HPP
