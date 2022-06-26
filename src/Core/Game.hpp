#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <map>
#include <memory>
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
     * Init application and set application directory
     * @param command: argv[0]
     */
    void init(const std::string& command);

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
     * Create window (windowed mode), with given dimension
     */
    void setResolution(unsigned int width, unsigned int height);

    /**
     * Create window (fullscreen mode)
     */
    void setFullscreen();

    bool isFullscreen() const;

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

    void setWindowProperties();

    sf::RenderWindow m_window;
    bool             m_fullscreen;
    bool             m_running;
    std::string      m_appDir;
    sf::View         m_view;

    // State management
    std::map<std::string, std::unique_ptr<State>> m_states;
    State* m_currentState;
};

#endif // GAME_HPP
