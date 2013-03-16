#ifndef WALLBREAKER_HPP
#define WALLBREAKER_HPP

#include <fstream>

#include "Screen.hpp"
#include "Core/Config.hpp"
#include "Entities/Brick.hpp"
#include "Entities/Paddle.hpp"
#include "Gui/BitmapText.hpp"
#include "Core/ParticleSystem.hpp"

class Wallbreaker: public Screen
{
public:
	enum Status { READY, PLAYING, GAME_OVER };

	Wallbreaker();
	~Wallbreaker();

	// override
	void onEvent(const sf::Event& event);

	// override
	void onFocus();

	// override
	void update(float frametime);

private:
	// override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void updateEntities(float frametime);

	bool checkBrick(Entity& entity, int i, int j);

	/**
	 * Load the next level in the level file
	 */
	bool loadNextLevel();

	void setStatus(Status status);


	// Delete all entities
	void clearEntities();

	// Create a new ball instance
	void createBall();


	static const int m_width  = NB_BRICK_COLS  * Brick::WIDTH;
	static const int m_height = NB_BRICK_LINES * Brick::HEIGHT;

	std::ifstream   m_level_file;
	Brick           m_bricks[NB_BRICK_LINES][NB_BRICK_COLS];
	int             m_nb_active_bricks;
	ParticleSystem& m_particles;
	BitmapText      m_info_text;
	sf::Sprite      m_hud_sprite;
	sf::Sprite      m_game_sprite;
	mutable sf::RenderTexture m_game_texture;

	typedef std::list<Entity*> EntityList;
	EntityList m_entities;
	Paddle     m_paddle;
	Status     m_status;
	int        m_player_lives;
	int        m_current_level;
};

#endif // WALLBREAKER_HPP
