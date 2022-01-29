#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "State.hpp"
#include "Core/ParticleSystem.hpp"
#include "Core/ParticleEmitter.hpp"
#include "Gui/BitmapText.hpp"
#include "Gui/Menu.hpp"


class MainMenu: public State
{
public:
    MainMenu();

    void onEvent(const sf::Event& event) override;

    void onFocus() override;

    void update(float frametime) override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Sprite m_background;
    sf::Sprite m_title;
    BitmapText m_aboutText;
    gui::Menu  m_menu;
    ParticleEmitter m_emitter;
    ParticleSystem m_particles;
};

#endif // MAIN_MENU_HPP
