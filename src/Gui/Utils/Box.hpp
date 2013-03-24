#ifndef GUI_BOX_HPP
#define GUI_BOX_HPP

#include <SFML/Graphics.hpp>

namespace gui
{

template <class T>
class Box: public sf::Drawable
{
public:
	Box();

	void prelight();
	void press();
	void release();
	void pack(float width, float height);
	void move(float dx, float dy);

	bool containsPoint(float x, float y) const;

	T          m_item;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool       m_pressed;

	sf::RectangleShape m_background;
};

}

#include "Box.inl"

#endif // GUI_BOX_HPP
