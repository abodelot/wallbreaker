#include "OptionsMenu.hpp"
#include "Core/Config.hpp"
#include "Core/Game.hpp"
#include "Core/Resources.hpp"
#include "Core/SoundSystem.hpp"
#include "Gui/Button.hpp"
#include "Gui/Label.hpp"


OptionsMenu::OptionsMenu():
    m_background(Resources::getTexture("background.png")), m_menu(Game::getInstance().getWindow())
{
    m_menu.setPosition(40, 40);
    m_menu.addLabel("Options")->setCharacterSize(2);

    // Create checkbox for turning music on/off
    m_ckMusic = new gui::CheckBox(SoundSystem::isMusicEnabled());
    m_ckMusic->setCallback([this]() { SoundSystem::enableMusic(m_ckMusic->isChecked()); });

    // Create checkbox for turning sound on/off
    m_ckSound = new gui::CheckBox(SoundSystem::isSoundEnabled());
    m_ckSound->setCallback([this]() {
        SoundSystem::enableSound(m_ckSound->isChecked());
        if (m_ckSound->isChecked())
        {
            SoundSystem::playSound("ball.ogg");
        }
    });

    // Create options box for selecting a resolution
    m_optResolution = new gui::OptionsBox<sf::Vector2u>;

    const sf::VideoMode& desktop = sf::VideoMode::getDesktopMode();
    sf::Vector2u resolution(APP_WIDTH, APP_HEIGHT);
    while (resolution.x < desktop.width && resolution.y < desktop.height)
    {
        addResolution(resolution);
        resolution.x += APP_WIDTH;
        resolution.y += APP_HEIGHT;
    }
    m_optResolution->addItem("Fullscreen", {0, 0}, Game::getInstance().isFullscreen());
    m_optResolution->setCallback([this]() {
        const sf::Vector2u& res = m_optResolution->getSelectedValue();
        if (res.x == 0 && res.y == 0)
            Game::getInstance().setFullscreen();
        else
            Game::getInstance().setResolution(res.x, res.y);
    });

    // Back button
    gui::Button* btnBack = new gui::Button("Back");
    btnBack->setCallback([]() { Game::getInstance().restorePreviousState(); });

    // Add widgets in form
    gui::Layout* form = m_menu.addLayout(gui::Layout::Form);
    form->addRow("Music:", m_ckMusic);
    form->addRow("Sound:", m_ckSound);
    form->addRow("Resolution:", m_optResolution);
    form->addRow("", btnBack);
}


void OptionsMenu::onEvent(const sf::Event& event)
{
    m_menu.onEvent(event);
}


void OptionsMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_background, states);
    target.draw(m_menu, states);
}


void OptionsMenu::addResolution(const sf::Vector2u& res)
{
    sf::Vector2u current = Game::getInstance().getWindow().getSize();
    m_optResolution->addItem(std::to_string(res.x) + "x" + std::to_string(res.y), res,
        res == current // Default selection is current resolution
    );
}
