#ifndef GUI_MENU_HPP
#define GUI_MENU_HPP

#include <vector>
#include <SFML/Graphics.hpp>

namespace gui
{

class Widget;
class Button;

class Menu
{
public:
	Menu(sf::RenderTarget& window);
	~Menu();

	/**
	 * Handle event and send it to widgets
	 * @return triggered widget ID, or -1 if none
	 */
	int onEvent(const sf::Event& event);

	/**
	 * Add a new button in the menu container
	 * The menu will take care of widget deallocation
	 * @return added widget
	 */
	Button* addButton(const sf::String& string, int id = -1);
	Widget* add(Widget* widget, int id = -1);
	Widget* add(const sf::String& label, Widget* widget, int id = -1);

	void setPosition(float x, float y);
	const sf::Vector2f& getPosition() const;

	void draw() const;

	void triggerCallback(const Widget* widget);

private:
	/**
	 * Get mouse cursor relative position
	 * @param x: absolute x position from the event
	 * @param y: absolute y position from the event
	 * @param relative: if not NULL, compute mouse position relative to this widget
	 * @return relative mouse position
	 */
	sf::Vector2f getMousePosition(int x, int y, const Widget* relative = NULL) const;

	typedef std::vector<Widget*> WidgetVector;

	sf::RenderTarget& m_window;
	sf::Vector2f      m_position;
	WidgetVector      m_widgets;
	Widget*           m_hover;
	const Widget*     m_triggered;
};

}

#endif // GUI_MENU_HPP
