#ifndef GUI_WIDGET_HPP
#define GUI_WIDGET_HPP

#include <SFML/Graphics.hpp>

namespace gui
{

/**
 * Base class for gui widgets
 */
class Widget: public sf::Drawable, public sf::Transformable
{
public:
	Widget(int id = -1);

	int getID() const;

	float getX() const { return getPosition().x; }
	float getY() const { return getPosition().y; }

	/**
	 * Get widget dimensions
	 */
	const sf::Vector2f& getSize() const;

	bool containsPoint(const sf::Vector2f& point) const;

	// callbacks ---------------------------------------------------------------

	virtual void onMouseEnter();
	virtual void onMouseLeave();
	virtual void onMouseMoved(float x, float y);
	virtual void onMousePressed(float x, float y);
	virtual void onMouseReleased(float x, float y);

protected:
	void setSize(const sf::Vector2f& size);

private:
	sf::Vector2f m_size;
	int          m_id;
};

}

#endif // GUI_WIDGET_HPP
