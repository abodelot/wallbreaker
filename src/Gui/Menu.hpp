#ifndef GUI_MENU_HPP
#define GUI_MENU_HPP

#include <vector>
#include <SFML/Graphics.hpp>

namespace gui
{

class Widget;

class Menu
{
public:
	Menu(sf::RenderTarget& window);
	~Menu();

	bool onEvent(const sf::Event& event, int& id);

	/**
	 * Add a new button in the menu container
	 * The menu will take care of widget deallocation
	 */
	void addButton(const sf::String& string, int id);

	void setPosition(float x, float y);

	void draw() const;

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
	WidgetVector      m_widgets;
	Widget*           m_hover;
	sf::Vector2f      m_position;
};

}

#endif // GUI_MENU_HPP
