#ifndef GUI_MENU_HPP
#define GUI_MENU_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "Button.hpp"

class Menu: public sf::Drawable
{
public:
	Menu();
	~Menu();

	bool onEvent(const sf::Event& event, int& id);

	void addButton(const sf::String& string, int id);

	void setPosition(float x, float y);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	typedef std::vector<Button*> WidgetVector;

	WidgetVector m_widgets;
	Button*      m_hover;
	sf::Vector2f m_position;
};

#endif // GUI_MENU_HPP
