#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class State;

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
     * Register a new game state
     */
    void addState(const std::string& name, State* state);

    /**
     * Set the next state to be displayed
     */
    void setCurrentState(const std::string& name);

    /**
     * Restore a previous state as the new current state
     */
    void restorePreviousState();

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

    // State management
    std::map<std::string, State*> m_states;
    State* m_currentState;
};

#endif // GAME_HPP
