#ifndef GUI_ARROW_HPP
#define GUI_ARROW_HPP

#include <SFML/Graphics.hpp>

namespace gui
{

/**
 * A simple arrow made of 3 points
 */
class Arrow: public sf::Drawable
{
public:
	enum Direction { Left, Right, Top, Bottom };

	Arrow();

	void build(float size, Direction direction);

	void setColor(const sf::Color& color);

	void move(float dx, float dy);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vertex m_triangle[3];
};

}

#endif // GUI_ARROW_HPP
