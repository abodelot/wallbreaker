#include "Game.hpp"
#include "Screens/Screen.hpp"
#include "Resources.hpp"
#include "Gui/Theme.hpp"


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
	//setResolution(APP_WIDTH, APP_HEIGHT);
	setResolution(APP_WIDTH * 2, APP_HEIGHT * 2);
	//setResolution(APP_WIDTH * 3, APP_HEIGHT * 3);
	m_app.setFramerateLimit(120);
	//m_app.setMouseCursorVisible(false);
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
	Resources::setDataPath(m_current_dir);

	// Init GUI theme
	gui::Theme::load(m_current_dir + "/resources/images/font.png");
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
				m_running = false;
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


void Game::setResolution(int width, int height)
{
	if (m_app.isOpen())
		m_app.close();

	m_app.create(sf::VideoMode(width, height), APP_TITLE, sf::Style::Close);
	m_view = sf::View(sf::FloatRect(0, 0, APP_WIDTH, APP_HEIGHT));
	m_app.setView(m_view);

	// Center window on desktop
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	m_app.setPosition(sf::Vector2i((desktop.width - m_app.getSize().x) / 2, (desktop.height - m_app.getSize().y) / 2));
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
	char current_time[256];
	time_t t = time(NULL);
	strftime(current_time, sizeof current_time, "%d-%m-%Y_%H-%M-%S", localtime(&t));

	std::string filename = m_current_dir + "/" + current_time + ".png";
	printf("screenshot saved to %s\n", filename.c_str());
	m_app.capture().saveToFile(filename);
}
