#include <iostream>
#include "Editor.hpp"


Editor::Editor():
	m_menu(Game::getInstance().getWindow()),
	m_show_grid(true)
{
	// Initialize render texture
	m_level_texture.create(m_width, m_height);

	// Sprites used for render textures
	m_level_sprite.setTexture(m_level_texture.getTexture());
	m_level_sprite.setPosition(GAME_BORDER_SIZE, GAME_BORDER_SIZE);

	// Create GUI menu
	m_menu.setPosition(80, m_height + GAME_BORDER_SIZE);
	m_menu.addButton("Back", 1);

	// Initialize bricks position
	for (int i = 0; i < NB_BRICK_LINES; ++i)
		for (int j = 0; j < NB_BRICK_COLS; ++j)
			m_bricks[i][j].setPosition(j * Brick::WIDTH, i * Brick::HEIGHT);

	// Initialize visual grid
	for (int i = 1; i < NB_BRICK_LINES; ++i)
	{
		m_grid_lines[i * 2].color        = sf::Color::Black;
		m_grid_lines[i * 2].position     = sf::Vector2f(0, Brick::HEIGHT * i);
		m_grid_lines[i * 2 + 1].color    = sf::Color::Black;
		m_grid_lines[i * 2 + 1].position = sf::Vector2f(m_width, Brick::HEIGHT * i);
	}

	for (int i = 1; i < NB_BRICK_COLS; ++i)
	{
		m_grid_cols[i * 2].color        = sf::Color::Black;
		m_grid_cols[i * 2].position     = sf::Vector2f(Brick::WIDTH * i, 0);
		m_grid_cols[i * 2 + 1].color    = sf::Color::Black;
		m_grid_cols[i * 2 + 1].position = sf::Vector2f(Brick::WIDTH * i, m_height);
	}

	updateTexture();

	m_cursor.setType(Brick::START);

	// Prelight cursor
	for (int i = 0; i < 4; ++i)
	{
		m_cursor_prelight[i].color = sf::Color(255, 255, 255, 128);
	}

}


void Editor::onEvent(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::MouseMoved:
		{
			// Convert mouse position and update cursor
			sf::Vector2f pos = Game::getInstance().getWindow().mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
			m_cursor.setPosition(pos);

			// Find brick index at the mouse position
			sf::Vector2i new_pos;
			new_pos.x = (pos.x - GAME_BORDER_SIZE) / Brick::WIDTH;
			new_pos.y = (pos.y - GAME_BORDER_SIZE) / Brick::HEIGHT;

			if (new_pos.x >= 0 && new_pos.x < NB_BRICK_COLS
			 && new_pos.y >= 0 && new_pos.y < NB_BRICK_LINES
			 && new_pos != m_cursor_pos)
			{
				std::cout << "x: " << new_pos.x << ", y: " << new_pos.y << std::endl;
				m_cursor_pos = new_pos;

				// Update prelight position
				m_cursor_prelight[0].position = {new_pos.x * Brick::WIDTH,       new_pos.y * Brick::HEIGHT};
				m_cursor_prelight[1].position = {(new_pos.x + 1) * Brick::WIDTH, new_pos.y * Brick::HEIGHT};
				m_cursor_prelight[2].position = {(new_pos.x + 1) * Brick::WIDTH, (new_pos.y + 1) * Brick::HEIGHT};
				m_cursor_prelight[3].position = {new_pos.x * Brick::WIDTH,       (new_pos.y + 1) * Brick::HEIGHT};

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					m_bricks[m_cursor_pos.y][m_cursor_pos.x].setType(m_cursor.getType());
					m_cursor.playSound();
				}
				updateTexture();
			}
			break;
		}
		case sf::Event::MouseButtonPressed:
			m_bricks[m_cursor_pos.y][m_cursor_pos.x].setType(m_cursor.getType());
			m_cursor.playSound();
			updateTexture();
			break;

		case sf::Event::MouseWheelMoved:
			if (event.mouseWheel.delta > 0)
				m_cursor.setType(m_cursor.getType() < Brick::UNBREAKABLE ? m_cursor.getType() + 1 : Brick::START);
			else
				m_cursor.setType(m_cursor.getType() > Brick::START ? m_cursor.getType() - 1 : Brick::UNBREAKABLE);
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::G)
			{
				m_show_grid = !m_show_grid;
				updateTexture();
			}
			break;
	}

	int id;
	if (m_menu.onEvent(event, id))
	{
		if (id == 1)
			Game::getInstance().previousScreen();
	}
}


void Editor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	target.draw(m_level_sprite);

	target.draw(m_cursor);

	// draw GUI menu
	m_menu.draw();
}


void Editor::updateTexture()
{
	m_level_texture.clear({0x16, 0x1e, 0x26});

	// Draw bricks
	for (int i = 0; i < NB_BRICK_LINES; ++i)
	{
		for (int j = 0; j < NB_BRICK_COLS; ++j)
		{
			const Brick& brick = m_bricks[i][j];
			if (brick.isActive())
				m_level_texture.draw(brick);
		}
	}
	m_level_texture.draw(m_cursor_prelight, 4, sf::Quads);

	// Draw grid
	if (m_show_grid)
	{
		m_level_texture.draw(m_grid_lines, NB_BRICK_LINES * 2, sf::Lines);
		m_level_texture.draw(m_grid_cols, NB_BRICK_COLS * 2, sf::Lines);
	}


	m_level_texture.display();
}
