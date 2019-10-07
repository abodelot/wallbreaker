#include "MainMenu.hpp"
#include "Core/Game.hpp"
#include "Core/Resources.hpp"
#include "Core/Config.hpp"
#include "Gui/Theme.hpp"

#define ABOUT_TEXT (APP_TITLE " " APP_VERSION " - " APP_URL " (Built " __DATE__ ", " __TIME__ ")")

MainMenu::MainMenu():
    m_background(Resources::getTexture("background.png")),
    m_aboutText(gui::Theme::font),
    m_menu(Game::getInstance().getWindow())
{
    m_title.setTexture(Resources::getTexture("title.png"));
    // Center title horizontally
    m_title.setPosition((APP_WIDTH - m_title.getLocalBounds().width) / 2, 16);

    m_aboutText.setString(ABOUT_TEXT);
    m_aboutText.setPosition(0, APP_HEIGHT - m_aboutText.getSize().y);
    m_aboutText.setColor(gui::Theme::textColor);

    m_menu.addButton("New game", 1);
    m_menu.addButton("Editor",   2);
    m_menu.addButton("Options",  3);
    m_menu.addButton("Quit",     4);
    // Center menu horizontally
    m_menu.setPosition((APP_WIDTH - m_menu.getSize().x) / 2, 100);

    m_emitter.setParticleColor({0x90, 0x50, 0x37});
    m_emitter.setLooping(true);
    m_emitter.setSpawnArea({0, 0, APP_WIDTH, APP_HEIGHT});
    m_emitter.setSpeed(10, 5);
    m_emitter.setTimeToLive(12);
    m_emitter.setParticleCount(150);
    m_emitter.setParticleSystem(m_particles);
    m_emitter.createParticles();
}


void MainMenu::onEvent(const sf::Event& event)
{
    switch (m_menu.onEvent(event))
    {
        case 1:
            Game::getInstance().setCurrentState("Wallbreaker");
            break;
        case 2:
            Game::getInstance().setCurrentState("Editor");
            break;
        case 3:
            Game::getInstance().setCurrentState("OptionsMenu");
            break;
        case 4:
            Game::getInstance().quit();
            break;
    }
}


void MainMenu::onFocus()
{
    Game::getInstance().getWindow().setMouseCursorVisible(true);
}


void MainMenu::update(float frametime)
{
    // About text is scrolled horizontally
    m_aboutText.move(-50 * frametime, 0);
    float width = m_aboutText.getSize().x;
    if (m_aboutText.getPosition().x + width < 0)
    {
        m_aboutText.setPosition(APP_WIDTH, m_aboutText.getPosition().y);
    }
    m_particles.update(frametime);
}


void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_background, states);
    target.draw(m_particles, states);
    target.draw(m_title, states);
    target.draw(m_aboutText, states);
    m_menu.show();
}
