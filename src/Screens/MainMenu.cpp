#include "MainMenu.hpp"
#include "Core/Game.hpp"
#include "Core/Resources.hpp"
#include "Core/Config.hpp"
#include "Gui/Theme.hpp"

#define ABOUT_TEXT (APP_TITLE " " APP_VERSION " - " APP_URL " (Built " __DATE__ ", " __TIME__ ")")

MainMenu::MainMenu():
    m_background(Resources::getTexture("background.png")),
    m_about_text(gui::Theme::getFont()),
    m_menu(Game::getInstance().getWindow())
{
    m_title.setTexture(Resources::getTexture("title.png"));
    m_title.setPosition(40, 16);

    m_about_text.setString(ABOUT_TEXT);
    m_about_text.setPosition(0, APP_HEIGHT - m_about_text.getSize().y);
    m_about_text.setColor(gui::Theme::BORDER_COLOR);

    m_menu.addButton("New game", 1);
    m_menu.addButton("Editor",   2);
    m_menu.addButton("Options",  3);
    m_menu.addButton("Quit",     4);
    // Center menu horizontally
    int x = (APP_WIDTH - m_menu.getSize().x) / 2;
    m_menu.setPosition(x, 100);
}


void MainMenu::onEvent(const sf::Event& event)
{
    switch (m_menu.onEvent(event))
    {
        case 1:
            Game::getInstance().setCurrentScreen("Wallbreaker");
            break;
        case 2:
            Game::getInstance().setCurrentScreen("Editor");
            break;
        case 3:
            Game::getInstance().setCurrentScreen("OptionsMenu");
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
    m_about_text.move(-50 * frametime, 0);
    float width = m_about_text.getSize().x;
    if (m_about_text.getPosition().x + width < 0)
    {
        m_about_text.setPosition(APP_WIDTH, m_about_text.getPosition().y);
    }
}


void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_background, states);
    target.draw(m_title, states);
    target.draw(m_about_text, states);
    m_menu.show();
}
