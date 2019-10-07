#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class Screen;

/**
 * Application singleton controller
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
    void setCurrentScreen(const std::string& id);

    /**
     * Restore a previous screen as the new current screen
     */
    void restorePreviousScreen();

    /**
     * Set application window dimension
     */
    void setResolution(size_t width, size_t height);

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

    void initGuiTheme();

    sf::RenderWindow m_window;
    bool             m_running;
    std::string      m_appDir;
    sf::View         m_view;

    // Screen management
    std::map<std::string, Screen*> m_screens;
    Screen*                        m_current_screen;
};

#endif // GAME_HPP
