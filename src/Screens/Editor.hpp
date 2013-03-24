#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "Screen.hpp"
#include "Core/Config.hpp"
#include "Core/Level.hpp"
#include "Entities/Brick.hpp"
#include "Gui/Menu.hpp"

class Editor: public Screen
{
public:
	Editor();

	// override
	void onEvent(const sf::Event& event);

private:
	// override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/**
	 * Update internal texture used for rendering current level
	 */
	void updateTexture();

	static const int  m_width  = NB_BRICK_COLS  * Brick::WIDTH;
	static const int  m_height = NB_BRICK_LINES * Brick::HEIGHT;

	Level             m_level;
	Brick             m_cursor;
	sf::Vertex        m_cursor_prelight[4];
	sf::Vector2i      m_cursor_pos;

	sf::RenderTexture m_level_texture;
	sf::Sprite        m_level_sprite;

	// 2 vertices per line
	sf::Vertex        m_grid_cols[NB_BRICK_COLS * 2];
	sf::Vertex        m_grid_lines[NB_BRICK_LINES * 2];
	bool              m_show_grid;

	gui::Menu  m_menu;
};

#endif // EDITOR_HPP
