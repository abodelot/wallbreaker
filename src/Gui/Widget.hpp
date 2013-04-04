#ifndef GUI_WIDGET_HPP
#define GUI_WIDGET_HPP

#include <SFML/Graphics.hpp>

namespace gui
{

enum State
{
	StateDefault,
	StateHovered,
	StateFocused,
};

class Menu;

/**
 * Abstract base class for gui widgets
 */
class Widget: public sf::Drawable, public sf::Transformable
{
public:
	Widget(int id = -1);

	/**
	 * Give an ID to the widget
	 */
	void setID(int id);
	int getID() const;

	inline float getX() const { return getPosition().x; }
	inline float getY() const { return getPosition().y; }

	/**
	 * Get widget dimensions
	 */
	const sf::Vector2f& getSize() const;

	/**
	 * Check if a point is inside the widget
	 */
	bool containsPoint(const sf::Vector2f& point) const;

	/**
	 * Check if the widget can be select and trigger events
	 */
	bool isSelectable() const;

	// callbacks ---------------------------------------------------------------

	virtual void onStateChanged(State state);
	virtual void onMouseMoved(float x, float y);
	virtual void onMousePressed(float x, float y);
	virtual void onMouseReleased(float x, float y);
	virtual void onKeyPressed(sf::Keyboard::Key key);
	virtual void onKeyReleased(sf::Keyboard::Key key);

protected:
	/**
	 * Set size occupied by the widget
	 */
	void setSize(const sf::Vector2f& size);

	void setSelectable(bool selectable);

	/**
	 * Notify parent that the widget has been triggered by user input
	 */
	void triggerCallback();

private:
	friend class Menu;
	void setParent(Menu* menu);

	Menu*        m_parent;
	sf::Vector2f m_size;
	int          m_id;
	bool         m_selectable;
};

}

#endif // GUI_WIDGET_HPP
