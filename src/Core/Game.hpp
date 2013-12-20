#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <map>
#include <stack>
#include <SFML/Graphics.hpp>

class Screen;

/**
 * Application controller
 */
class Game
{
public:
	/**
	 * Get application controller singleton instance
	 */
	static Game& getInstance();

	/**
	 * Init application and set current working directory
	 */
	void init(const std::string& path);

	/**
	 * Enter application main loop
	 */
	void run();

	/**
	 * Exit application
	 */
	void quit();

	/**
	 * Register a new screen
	 */
	void addScreen(const std::string& id, Screen* screen);

	/**
	 * Set the next screen to be displayed
	 */
	void nextScreen(const std::string& id);

	/**
	 * Go back to the previous displayed screen
	 */
	void previousScreen();

	/**
	 * Set application window dimension
	 */
	void setResolution(size_t width, size_t height);

	/**
	 * Get application root directory
	 */
	const std::string& getApplicationDir() const;

	/**
	 * Get application render window
	 */
	sf::RenderWindow& getWindow();

	/**
	 * Save a picture of the screen
	 */
	void takeScreenshot() const;

private:
	Game();
	Game(const Game&);
	~Game();

	sf::RenderWindow m_window;
	bool             m_running;
	std::string      m_app_dir;
	sf::View         m_view;

	// Screen management
	std::map<std::string, Screen*> m_screens;
	std::stack<Screen*>            m_screens_history;
	Screen*                        m_current_screen;
};

#endif // GAME_HPP
