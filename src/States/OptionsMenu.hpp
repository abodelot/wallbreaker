#ifndef OPTIONSMENU_HPP
#define OPTIONSMENU_HPP

#include "State.hpp"
#include "Gui/Menu.hpp"
#include "Gui/OptionsBox.hpp"
#include "Gui/Button.hpp"
#include "Gui/CheckBox.hpp"

/**
 * Menu dedicated to settings management
 */
class OptionsMenu: public State
{
public:
    OptionsMenu();

    void onEvent(const sf::Event& event) override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    /**
     * Add a resolution in the option box widget
     */
    void addResolution(const sf::Vector2u& res);

    sf::Sprite m_background;
    // Gui elements
    gui::Menu                      m_menu;
    gui::CheckBox*                 m_ckMusic;
    gui::CheckBox*                 m_ckSound;
    gui::OptionsBox<sf::Vector2u>* m_optResolution;
};

#endif // OPTIONSMENU_HPP
