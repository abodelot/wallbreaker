#ifndef WALLBREAKER_HPP
#define WALLBREAKER_HPP

#include "Screen.hpp"
#include "Core/Config.hpp"
#include "Core/ParticleSystem.hpp"
#include "Core/Level.hpp"
#include "Entities/Paddle.hpp"
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

	// override
	void onEvent(const sf::Event& event);

	// override
	void update(float frametime);

	// Create a new ball instance
	void createBall();

	void addPlayerLife();

private:
	// override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool loadNextLevel();

	void updateTexture();

	void updateEntities(float frametime);

	bool checkBrick(Entity& entity, int i, int j, const sf::Vector2f& old_pos);

	void setStatus(Status status);

	void addEntity(Entity* entity);

	// Delete all entities
	void clearEntities();

	static const int m_width  = NB_BRICK_COLS  * Brick::WIDTH;
	static const int m_height = NB_BRICK_LINES * Brick::HEIGHT;

	Level             m_level;
	int               m_remaining_bricks;
	ParticleSystem&   m_particles;
	BitmapText        m_info_text;
	sf::Sprite        m_hud_sprite;
	sf::Sprite        m_level_sprite;
	sf::RenderTexture m_level_texture;

	typedef std::list<Entity*> EntityList;
	EntityList m_entities;
	Paddle     m_paddle;
	Status     m_status;
	int        m_player_lives;

	// GUI elements for pause
	gui::Menu m_menu;
};

#endif // WALLBREAKER_HPP
