#include <iostream>
#include "Game.hpp"
#include "Config.hpp"
#include "Screens/Screen.hpp"
#include "Resources.hpp"
#include "Settings.hpp"
#include "SoundSystem.hpp"
#include "Gui/Theme.hpp"
#include "Utils/IniParser.hpp"


Game& Game::getInstance()
{
	static Game self;
	return self;
}


Game::Game():
	m_running(true),
	m_current_screen(NULL),
	m_next_screen(NULL)
{
}


Game::~Game()
{
	while (!m_screen_history.empty())
	{
		delete m_screen_history.top();
		m_screen_history.pop();
	}
}


void Game::init(const std::string& path)
{
	// Set current directory
	size_t found = path.find_last_of("/\\");
	m_current_dir = path.substr(0, found + 1);
	if (m_current_dir.empty())
		m_current_dir = "./";

	// Init resources search directory
	Resources::setSearchPath(m_current_dir + "/resources/");

	// Init GUI theme
	gui::Theme::loadFont(m_current_dir + "/resources/images/font.png");

	// Load configuration from settings file
	IniParser config;
	if (config.load(m_current_dir + SETTINGS_FILE))
	{
		std::cout << "* loading settings from " << SETTINGS_FILE << std::endl;

		config.seek_section("Wallbreaker");
		size_t app_width = config.get("app_width", APP_WIDTH * 2);
		size_t app_height = config.get("app_height", APP_HEIGHT * 2);
		setResolution(app_width, app_height);

		Settings::highscore = config.get("highscore", 0);

		SoundSystem::enableSound(config.get("sound", true));
	}
	else
	{
		setResolution(APP_WIDTH * 2, APP_HEIGHT * 2);
	}
}


void Game::run()
{
	sf::Clock clock;

	while (m_running)
	{
		// Check for next screen
		if (m_next_screen != NULL)
		{
			if (!m_screen_history.empty() && m_next_screen == m_screen_history.top())
			{
				delete m_current_screen;
				m_screen_history.pop();
			}
			else
			{
				m_screen_history.push(m_current_screen);
			}
			m_current_screen = m_next_screen;
			m_current_screen->onFocus();
			m_next_screen = NULL;
		}

		// Poll events and send them to the current screen
		sf::Event event;
		while (m_app.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				quit();
			}
			else
			{
				m_current_screen->onEvent(event);
			}
		}

		// Update current screen
		float frametime = clock.restart().asSeconds();
		m_current_screen->update(frametime);

		// Render current screen
		m_app.clear();
		m_app.setView(m_view);
		m_app.draw(*m_current_screen);
		m_app.display();
	}
}


void Game::quit()
{
	m_running = false;

	// Save configuration to settings file
	IniParser config;
	config.seek_section("Wallbreaker");
	config.set("highscore",  Settings::highscore);
	config.set("app_width",  m_app.getSize().x);
	config.set("app_height", m_app.getSize().y);
	config.set("sound",      SoundSystem::isSoundEnabled());
	config.save(m_current_dir + SETTINGS_FILE);
}


void Game::nextScreen(Screen* screen)
{
	m_next_screen = screen;
}


void Game::previousScreen()
{
	if (!m_screen_history.empty())
	{
		m_next_screen = m_screen_history.top();
	}
}


void Game::setResolution(size_t width, size_t height)
{
	if (m_app.isOpen())
		m_app.close();

	m_app.create(sf::VideoMode(width, height), APP_TITLE, sf::Style::Close);
	m_view = sf::View(sf::FloatRect(0, 0, APP_WIDTH, APP_HEIGHT));
	m_app.setView(m_view);
	m_app.setFramerateLimit(60);
	m_app.setKeyRepeatEnabled(false);

	// Center window on desktop
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	m_app.setPosition({(desktop.width - width) / 2, (desktop.height - height) / 2});

	// Set application icon
	static sf::Image icon = Resources::getTexture("application-icon.png").copyToImage();
	m_app.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}


const std::string& Game::getCurrentDir() const
{
	return m_current_dir;
}


sf::RenderWindow& Game::getWindow()
{
	return m_app;
}


void Game::takeScreenshot() const
{
	char current_time[20]; // YYYY-MM-DD_HH-MM-SS + \0
	time_t t = time(NULL);
	strftime(current_time, sizeof current_time, "%Y-%m-%d_%H-%M-%S", localtime(&t));
	std::string filename = m_current_dir + "screenshots/" + current_time + ".png";

	if (m_app.capture().saveToFile(filename))
		std::cout << "* screenshot saved to " << filename << std::endl;
	else
		std::cerr << "* couldn't save screenshot " << filename << std::endl;
}
