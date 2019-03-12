#ifndef WALLBREAKER_HPP
#define WALLBREAKER_HPP

#include "Screen.hpp"
#include "Core/Config.hpp"
#include "Core/ParticleSystem.hpp"
#include "Core/LevelManager.hpp"
#include "Core/HUD.hpp"
#include "Entities/Paddle.hpp"
#include "Entities/Ball.hpp"
#include "Gui/BitmapText.hpp"
#include "Gui/Menu.hpp"

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

    bool checkBrick(Entity& entity, int i, int j, const sf::Vector2f& old_pos);

    void setStatus(Status status);

    const int         m_width;
    const int         m_height;
    LevelManager&     m_level;
    int               m_remaining_bricks;
    ParticleSystem&   m_particles;
    BitmapText        m_info_text;
    sf::Sprite        m_hud_sprite;
    HUD               m_hud;
    sf::Sprite        m_borders_sprite;
    sf::Sprite        m_background_sprite;
    sf::Sprite        m_level_sprite;
    sf::RenderTexture m_level_texture;

    int               m_score;

    typedef std::list<Entity*> EntityList;
    EntityList m_entities;
    Paddle     m_paddle;
    Status     m_status;
    int        m_player_lives;

    // GUI elements for menus
    gui::Menu m_pause_menu;
    gui::Menu m_game_over_menu;
};

#endif // WALLBREAKER_HPP
