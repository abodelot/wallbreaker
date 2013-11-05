#include <iostream>
#include "Game.hpp"
#include "Config.hpp"
#include "Screens/Screen.hpp"
#include "Resources.hpp"
#include "Settings.hpp"
#include "SoundSystem.hpp"
#include "Gui/Theme.hpp"
#include "Utils/IniParser.hpp"
#include "Utils/FileSystem.hpp"


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
	// Set application directory
	size_t last_dir = path.find_last_of("/\\");
	m_app_dir = path.substr(0, last_dir + 1);
	if (m_app_dir.empty())
		m_app_dir = "./";

	// Init resources search directory
	Resources::setSearchPath(m_app_dir + "/resources/");

	// Init GUI theme
	gui::Theme::loadFont(m_app_dir + "/resources/images/font.png");

	// Load configuration from settings file
	IniParser config;
	if (config.load(m_app_dir + SETTINGS_FILE))
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
		while (m_window.pollEvent(event))
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
		m_window.clear();
		m_window.setView(m_view);
		m_window.draw(*m_current_screen);
		m_window.display();
	}
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
	config.save(m_app_dir + SETTINGS_FILE);
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
	if (m_window.isOpen())
		m_window.close();

	m_window.create(sf::VideoMode(width, height), APP_TITLE, sf::Style::Close);
	m_view = sf::View(sf::FloatRect(0, 0, APP_WIDTH, APP_HEIGHT));
	m_window.setView(m_view);
	m_window.setFramerateLimit(60);
	m_window.setKeyRepeatEnabled(false);

	// Center window on desktop
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	m_window.setPosition({(desktop.width - width) / 2, (desktop.height - height) / 2});

	// Set application icon
	static sf::Image icon = Resources::getTexture("application-icon.png").copyToImage();
	m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}


const std::string& Game::getApplicationDir() const
{
	return m_app_dir;
}


sf::RenderWindow& Game::getWindow()
{
	return m_window;
}


void Game::takeScreenshot() const
{
	// Create screenshots directory if it doesn't exist yet
	std::string screenshot_path = m_app_dir + SCREENSHOT_DIR;
	if (!FileSystem::isDirectory(screenshot_path))
		FileSystem::createDirectory(screenshot_path);

	char current_time[20]; // YYYY-MM-DD_HH-MM-SS + \0
	time_t t = time(NULL);
	strftime(current_time, sizeof current_time, "%Y-%m-%d_%H-%M-%S", localtime(&t));
	std::string filename = screenshot_path + "/" + current_time + ".png";

	if (m_window.capture().saveToFile(filename))
		std::cout << "screenshot saved to " << filename << std::endl;
}
