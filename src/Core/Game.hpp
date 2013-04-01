#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <stack>
#include <SFML/Graphics.hpp>

class Screen;

/**
 * Application controller
 */
class Game
{
public:
	/// Get singleton instance
	static Game& getInstance();

	/// Init application and set current working directory
	void init(const std::string& path);

	/// Enter application main loop
	void run();

	/// Exit application
	void quit();

	/// Set next screen to be displayed
	void nextScreen(Screen* screen);

	/// Go back to the previous displayed screen
	void previousScreen();

	/// Set application window dimension
	void setResolution(int width, int height);

	/// Get execution current directory
	const std::string& getCurrentDir() const;

	/// Get application render window
	sf::RenderWindow& getWindow();

	/// Save a picture of the screen
	void takeScreenshot() const;

	sf::View& getView() { return m_view; }
private:
	Game();
	Game(const Game&);
	~Game();

	sf::RenderWindow m_app;
	bool             m_running;
	std::string      m_current_dir;
	sf::View         m_view;

	// Screen management
	typedef std::stack<Screen*> ScreenStack;
	ScreenStack m_screen_history;
	Screen*     m_current_screen;
	Screen*     m_next_screen;
};

#endif // GAME_HPP
