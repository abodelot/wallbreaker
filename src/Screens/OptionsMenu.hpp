#ifndef OPTIONSMENU_HPP
#define OPTIONSMENU_HPP

#include "Screen.hpp"
#include "Gui/Menu.hpp"
#include "Gui/OptionsBox.hpp"
#include "Gui/Button.hpp"
#include "Gui/CheckBox.hpp"

/**
 * Menu dedicated to settings management
 */
class OptionsMenu: public Screen
{
public:
	OptionsMenu();

	// override
	void onEvent(const sf::Event& event);

private:
	// override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/**
	 * Add a resolution in the option box widget
	 */
	void addResolution(const sf::Vector2u& res);

	// Gui elements
	gui::Menu      m_menu;
	gui::CheckBox* m_ck_sound;
	gui::OptionsBox<sf::Vector2u>* m_opt_resolution;
};

#endif // OPTIONSMENU_HPP
