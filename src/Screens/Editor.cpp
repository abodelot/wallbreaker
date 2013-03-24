#include <iostream>
#include "Editor.hpp"
#include "Gui/OptionsBox.hpp"
#include "Gui/Button.hpp"

Editor::Editor():
	m_show_grid(true),
	m_menu(Game::getInstance().getWindow())
{
	// Initialize render texture
	m_level_texture.create(m_width, m_height);

	// Sprites used for render textures
	m_level_sprite.setTexture(m_level_texture.getTexture());
	m_level_sprite.setPosition(GAME_BORDER_SIZE, GAME_BORDER_SIZE);

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

	// Create GUI menu
	m_menu.setPosition(20, m_height + GAME_BORDER_SIZE + 2);


	m_opt_levels = new gui::OptionsBox<size_t>;
	for (size_t i = 1; i <= m_level.getLevelCount(); ++i)
		m_opt_levels->addItem("Level " + std::to_string(i), i);

	m_menu.add(m_opt_levels,      1);
	m_menu.addButton("Save",      2);
	m_menu.addButton("Reload",    3);
	m_menu.addButton("New level", 4)->setPosition(130, m_menu.getPosition().y);
	m_but_grid = m_menu.addButton("Grid off", 5);
	m_menu.addButton("Back",      6);
}


void Editor::onEvent(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::MouseMoved:
		{
			// Convert mouse position and update cursor
			sf::Vector2f mouse = Game::getInstance().getWindow().mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
			m_cursor.setPosition(mouse);

			// Find brick index at the mouse position
			sf::Vector2i new_pos;
			new_pos.x = int(mouse.x - GAME_BORDER_SIZE) / Brick::WIDTH;
			new_pos.y = int(mouse.y - GAME_BORDER_SIZE) / Brick::HEIGHT;

			if (new_pos.x >= 0 && new_pos.x < NB_BRICK_COLS
			 && new_pos.y >= 0 && new_pos.y < NB_BRICK_LINES
			 && new_pos != m_cursor_pos)
			{
				// Update prelight position
				m_cursor_prelight[0].position = {new_pos.x * Brick::WIDTH,       new_pos.y * Brick::HEIGHT};
				m_cursor_prelight[1].position = {(new_pos.x + 1) * Brick::WIDTH, new_pos.y * Brick::HEIGHT};
				m_cursor_prelight[2].position = {(new_pos.x + 1) * Brick::WIDTH, (new_pos.y + 1) * Brick::HEIGHT};
				m_cursor_prelight[3].position = {new_pos.x * Brick::WIDTH,       (new_pos.y + 1) * Brick::HEIGHT};

				setBrick(new_pos);
				updateTexture();
			}
			m_cursor_pos = new_pos;

			break;
		}
		case sf::Event::MouseButtonPressed:
			if (m_cursor_pos.x >= 0 && m_cursor_pos.x < NB_BRICK_COLS
			 && m_cursor_pos.y >= 0 && m_cursor_pos.y < NB_BRICK_LINES)
			{
				setBrick(m_cursor_pos);
				updateTexture();
			}
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
				toggleGrid();
			}
			break;

		default:
			break;
	}

	// Handle GUI events
	switch (m_menu.onEvent(event))
	{
		case 1: // Level selector
		{
			int level = m_opt_levels->getSelectedValue();
			if (m_level.loadAt(level))
			{
				updateTexture();
			}
			break;
		}
		case 2: // Save
			m_level.save();
			break;
		case 3: // Reload
			m_level.reload();
			updateTexture();
			break;
		case 4: // New level
		{
			size_t index = m_level.append();
			// Insert new level in level list
			m_opt_levels->addItem("Level " + std::to_string(index), index, true);
			updateTexture();
			break;
		}
		case 5: // Grid on/off
			toggleGrid();
			break;
		case 6: // Back
			Game::getInstance().previousScreen();
			break;
	}
}


void Editor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_level_sprite);
	target.draw(m_cursor);

	// Draw GUI menu
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
			Brick& brick = m_level.getBrick(i, j);
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


void Editor::toggleGrid()
{
	m_show_grid = !m_show_grid;
	if (m_show_grid)
		m_but_grid->setString("Grid off");
	else
		m_but_grid->setString("Grid on");

	updateTexture();
}


void Editor::setBrick(const sf::Vector2i& index)
{
	// Put with left click, erase with right click
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		Brick& b = m_level.getBrick(index.y, index.x);
		if (b.getType() != m_cursor.getType())
		{
			b.setType(m_cursor.getType());
			m_cursor.playSound();
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		m_level.getBrick(index.y, index.x).setType(Brick::NONE);
	}
}
