#include "Game.hpp"
#include "Config.hpp"
#include "Resources.hpp"
#include "LevelManager.hpp"
#include "Settings.hpp"
#include "SoundSystem.hpp"
#include "States/State.hpp"
#include "Gui/Theme.hpp"
#include "Utils/IniParser.hpp"
#include "Utils/FileSystem.hpp"
#include <iostream>


Game& Game::getInstance()
{
    static Game self;
    return self;
}


Game::Game():
    m_running(true),
    m_currentState(nullptr)
{
}


Game::~Game()
{
}


void Game::init(const std::string& path)
{
    // Set application directory: compute dirname from path
    m_appDir = filesystem::dirname(path);

    // Init resources search directory
    Resources::setSearchPath(m_appDir + "/resources/");

    // Init levels
    std::cout << "* loading levels from " << APP_LEVEL_FILE << std::endl;
    LevelManager::getInstance().openFromFile(m_appDir + APP_LEVEL_FILE);

    initGuiTheme();

    SoundSystem::openMusicFromFile(m_appDir + "/resources/musics/evolution_sphere.ogg");

    // Load configuration from settings file
    IniParser config;
    if (config.load(m_appDir + APP_SETTINGS_FILE))
    {
        std::cout << "* loading settings from " << APP_SETTINGS_FILE << std::endl;
        config.seek_section("Wallbreaker");
        size_t app_width = config.get("app_width", APP_WIDTH * 2);
        size_t app_height = config.get("app_height", APP_HEIGHT * 2);
        setResolution(app_width, app_height);

        Settings::highscore = config.get("highscore", 0);

        SoundSystem::enableSound(config.get("sound", true));
        SoundSystem::enableMusic(config.get("music", true));
    }
    else
    {
        setResolution(APP_WIDTH * 2, APP_HEIGHT * 2);
    }

    SoundSystem::playMusic();
}


void Game::run()
{
    sf::Clock clock;

    while (m_running)
    {
        // Poll events and send them to the current state
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                quit();
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F2)
            {
                takeScreenshot();
            }
            else
            {
                m_currentState->onEvent(event);
            }
        }

        // Update current state
        float frametime = clock.restart().asSeconds();
        m_currentState->update(frametime);

        // Render current state
        m_window.clear();
        m_window.setView(m_view);
        m_window.draw(*m_currentState);
        m_window.display();
    }
}


void Game::initGuiTheme()
{
    // Load assets for GUI
    gui::Theme::font.loadFromFile(m_appDir + "/resources/images/font.png");
    gui::Theme::clickSound.setBuffer(Resources::getSoundBuffer("click.ogg"));

    gui::Theme::textColor = gui::hexToColor("#d9d9f5");
    gui::Theme::backgroundColor = gui::hexToColor("#573062");
    gui::Theme::hoverColor = gui::modulateColor(gui::Theme::backgroundColor, 1.3);
    gui::Theme::focusColor = gui::modulateColor(gui::Theme::backgroundColor, 0.7);
    gui::Theme::topBorderColor = gui::hexToColor("#bd37a0");
    gui::Theme::bottomBorderColor = gui::modulateColor(gui::Theme::topBorderColor, 0.7);
}


void Game::quit()
{
    m_running = false;

    // Save configuration to settings file
    IniParser config;
    config.seek_section("Wallbreaker");
    config.set("highscore",  Settings::highscore);
    config.set("app_width",  m_window.getSize().x);
    config.set("app_height", m_window.getSize().y);
    config.set("sound",      SoundSystem::isSoundEnabled());
    config.set("music",      SoundSystem::isMusicEnabled());
    config.save(m_appDir + APP_SETTINGS_FILE);

    SoundSystem::stopAll();
}


void Game::addState(const std::string& id, State* state)
{
    m_states.emplace(id, std::unique_ptr<State>(state));
}


void Game::setCurrentState(const std::string& name)
{
    State* previous = m_currentState;
    m_currentState = m_states.at(name).get();
    m_currentState->setPrevious(previous);
    m_currentState->onFocus();
}


void Game::restorePreviousState()
{
    if (m_currentState && m_currentState->getPrevious())
    {
        m_currentState = m_currentState->getPrevious();
        m_currentState->onFocus();
    }
    else
    {
        std::cerr << "[Game] No previous state available" << std::endl;
    }
}


void Game::setResolution(size_t width, size_t height)
{
    if (m_window.isOpen())
    {
        m_window.close();
    }
    m_window.create(sf::VideoMode(width, height), APP_TITLE, sf::Style::Close);
    m_view = sf::View(sf::FloatRect(0, 0, APP_WIDTH, APP_HEIGHT));
    m_window.setView(m_view);
    m_window.setFramerateLimit(APP_FPS);
    m_window.setKeyRepeatEnabled(false);

    // Center window on desktop
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    m_window.setPosition(sf::Vector2i((desktop.width - width) / 2, (desktop.height - height) / 2));
    // Set application icon
    sf::Image icon = Resources::getTexture("application-icon.png").copyToImage();
    m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}


sf::RenderWindow& Game::getWindow()
{
    return m_window;
}


void Game::takeScreenshot() const
{
    // Create screenshots directory if it doesn't exist yet
    std::string screenshotPath = m_appDir + APP_SCREENSHOT_DIR;
    if (!filesystem::is_directory(screenshotPath))
    {
        filesystem::create_directory(screenshotPath);
    }

    char currentTime[20]; // YYYY-MM-DD_HH-MM-SS + \0
    time_t t = time(NULL);
    strftime(currentTime, sizeof currentTime, "%Y-%m-%d_%H-%M-%S", localtime(&t));
    std::string filename = screenshotPath + "/" + currentTime + ".png";

    sf::Texture texture;
    texture.create(m_window.getSize().x, m_window.getSize().y);
    texture.update(m_window);
    if (texture.copyToImage().saveToFile(filename))
    {
        std::cout << "screenshot saved to " << filename << std::endl;
    }
}
