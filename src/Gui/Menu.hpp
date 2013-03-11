#ifndef GUI_MENU_HPP
#define GUI_MENU_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "Button.hpp"

class Menu
{
public:
	Menu(sf::RenderTarget& window);
	~Menu();

	bool onEvent(const sf::Event& event, int& id);

	void addButton(const sf::String& string, int id);

	void setPosition(float x, float y);

	void draw() const;

private:
	typedef std::vector<Button*> WidgetVector;

	sf::RenderTarget& m_window;
	WidgetVector      m_widgets;
	Button*           m_hover;
	sf::Vector2f      m_position;
};

#endif // GUI_MENU_HPP
