#ifndef WALLBREAKER_HPP
#define WALLBREAKER_HPP

#include "Screen.hpp"
#include "Core/Config.hpp"
#include "Core/HUD.hpp"
#include "Entities/Paddle.hpp"
#include "Entities/Ball.hpp"
#include "Gui/BitmapText.hpp"
#include "Gui/Menu.hpp"

class LevelManager;
class ParticleSystem;

/**
 * Game screen
 */
class Wallbreaker: public Screen
{
public:
    enum Status { READY, PLAYING, PAUSED, GAME_OVER };

    Wallbreaker();
    ~Wallbreaker();

    void onEvent(const sf::Event& event) override;

    void update(float frametime) override;

    // Create a new ball instance
    void createBall();

    void addPlayerLife();

    void applyOnEachBall(Ball::ActionPointer action);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void resetGame(size_t level=1);

    void loadNextLevel();

    void updateLevelTexture();

    void addEntity(Entity* entity);

    // Delete all entities
    void clearEntities();

    void updateEntities(float frametime);

    bool checkBrick(Entity& entity, int i, int j, const sf::Vector2f& oldPos);

    void setStatus(Status status);

    const int         m_width;
    const int         m_height;
    LevelManager&     m_level;
    int               m_remainingBricks;
    ParticleSystem&   m_particles;
    BitmapText        m_infoText;
    sf::Sprite        m_hudSprite;
    HUD               m_hud;
    sf::Sprite        m_bordersSprite;
    sf::Sprite        m_backgroundSprite;
    sf::Sprite        m_levelSprite;
    sf::RenderTexture m_levelTexture;

    int               m_score;

    typedef std::list<Entity*> EntityList;
    EntityList m_entities;
    Paddle     m_paddle;
    Status     m_status;
    int        m_playerLives;

    // GUI elements for menus
    gui::Menu m_pauseMenu;
    gui::Menu m_gameOverMenu;
};

#endif // WALLBREAKER_HPP
