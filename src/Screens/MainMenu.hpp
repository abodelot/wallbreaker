#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "Screen.hpp"
#include "Gui/BitmapText.hpp"
#include "Gui/Menu.hpp"


class MainMenu: public Screen
{
public:
	MainMenu();

	void onEvent(const sf::Event& event) override;

	void onFocus() override;

	void update(float frametime) override;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Sprite m_title;
	BitmapText m_about_text;
	gui::Menu  m_menu;
};

#endif // MAIN_MENU_HPP
