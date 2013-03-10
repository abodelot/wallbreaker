#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "Screen.hpp"
#include "Gui/BitmapText.hpp"
#include "Gui/Menu.hpp"

class MainMenu: public Screen
{
public:
	MainMenu();

	// override
	void onEvent(const sf::Event& event);

	// override
	void onFocus();

	// override
	void update(float frametime);

private:
	// override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Sprite m_title;
	BitmapText   m_about_text;
	//sf::View m_view;

	Menu m_menu;
};

#endif // MAIN_MENU_HPP
