#include "Editor.hpp"
#include "Core/Game.hpp"
#include "Core/Resources.hpp"
#include "Gui/Button.hpp"
#include "Gui/CheckBox.hpp"
#include "Gui/Label.hpp"
#include "Gui/OptionsBox.hpp"
#include <iostream>


Editor::Editor():
    m_background(Resources::getTexture("background.png")),
    m_level(LevelManager::getInstance()),
    m_show_grid(true),
    m_menu(Game::getInstance().getWindow())
{
    // Initialize render texture
    m_level_texture.create(m_width, m_height);

    // Sprites used for render textures
    m_level_sprite.setTexture(m_level_texture.getTexture());
    m_level_sprite.setPosition(LevelManager::BORDER_SIZE, LevelManager::BORDER_SIZE);

    m_borders_sprite.setTexture(Resources::getTexture("borders.png"));

    // Initialize visual grid (+0.5 for portable pixel-perfect rendition)
    const sf::Color gridColor(0x5f, 0x5f, 0x5f);
    for (int i = 1; i < LevelManager::NB_BRICK_LINES; ++i)
    {
        m_grid_lines[i * 2].color = gridColor;
        m_grid_lines[i * 2].position = sf::Vector2f(0.5, Brick::HEIGHT * i - 0.5);
        m_grid_lines[i * 2 + 1].color = gridColor;
        m_grid_lines[i * 2 + 1].position = sf::Vector2f(m_width + 0.5, Brick::HEIGHT * i - 0.5);
    }

    for (int i = 1; i < LevelManager::NB_BRICK_COLS; ++i)
    {
        m_grid_cols[i * 2].color = gridColor;
        m_grid_cols[i * 2].position = sf::Vector2f(Brick::WIDTH * i - 0.5, 0.5);
        m_grid_cols[i * 2 + 1].color = gridColor;
        m_grid_cols[i * 2 + 1].position = sf::Vector2f(Brick::WIDTH * i - 0.5, m_height + 0.5);
    }

    updateTexture();

    m_cursor.setType(Brick::START);

    // Prelight cursor
    for (int i = 0; i < 4; ++i)
    {
        m_cursor_prelight[i].color = sf::Color(255, 255, 255, 128);
    }

    // Create GUI menu
    m_menu.setPosition(m_width + LevelManager::BORDER_SIZE * 2 + 4, LevelManager::BORDER_SIZE);
    m_menu.addLabel(" Level Editor");

    // Populate level list
    m_opt_levels = new gui::OptionsBox<size_t>;
    for (size_t i = 1; i <= m_level.getLevelCount(); ++i)
        m_opt_levels->addItem("Level " + std::to_string(i), i);

    m_opt_levels->setCallback([this]() {
        size_t level = m_opt_levels->getSelectedValue();
        if (m_level.loadAt(level))
        {
            updateTexture();
        }
    });
    m_menu.add(m_opt_levels);

    m_menu.addButton("Save", [this]() { m_level.save(); });

    m_menu.addButton("Reload", [this]() {
        m_level.reload();
        updateTexture();
    });

    m_menu.addButton("New level", [this]() {
        size_t index = m_level.append();
        // Insert new level in level list
        m_opt_levels->addItem("Level " + std::to_string(index), index, true);
        updateTexture();
    });

    // Toggle the editor grid on/off
    m_ck_grid = new gui::CheckBox(m_show_grid);
    m_ck_grid->setCallback([this]() { toggleGrid(); });
    gui::Layout* form = m_menu.addLayout(gui::Layout::Form);
    form->addRow("Show grid:", m_ck_grid);

    m_menu.addButton("Back", []() { Game::getInstance().restorePreviousState(); });
}


void Editor::onEvent(const sf::Event& event)
{
    switch (event.type)
    {
    case sf::Event::MouseMoved:
    {
        // Convert mouse position and update cursor coords
        sf::Vector2f mouse = Game::getInstance().getWindow().mapPixelToCoords(
            {event.mouseMove.x, event.mouseMove.y});
        m_cursor.setPosition(mouse.x, mouse.y);

        // Find brick index at the mouse position
        sf::Vector2i coords;
        coords.x = int(mouse.x - LevelManager::BORDER_SIZE) / Brick::WIDTH;
        coords.y = int(mouse.y - LevelManager::BORDER_SIZE) / Brick::HEIGHT;

        if (coords.x >= 0 && coords.x < LevelManager::NB_BRICK_COLS && coords.y >= 0
            && coords.y < LevelManager::NB_BRICK_LINES && coords != m_cursor_coords)
        {
            // Update prelight position
            float left = coords.x * Brick::WIDTH;
            float top = coords.y * Brick::HEIGHT;
            float right = (coords.x + 1) * Brick::WIDTH;
            float bottom = (coords.y + 1) * Brick::HEIGHT;
            m_cursor_prelight[0].position = {left, top};
            m_cursor_prelight[1].position = {right, top};
            m_cursor_prelight[2].position = {right, bottom};
            m_cursor_prelight[3].position = {left, bottom};

            setBrick(coords);
            updateTexture();
        }
        m_cursor_coords = coords;

        break;
    }
    case sf::Event::MouseButtonPressed:
        if (m_cursor_coords.x >= 0 && m_cursor_coords.x < LevelManager::NB_BRICK_COLS
            && m_cursor_coords.y >= 0 && m_cursor_coords.y < LevelManager::NB_BRICK_LINES)
        {
            setBrick(m_cursor_coords);
            updateTexture();
        }
        break;

    case sf::Event::MouseWheelMoved:
        if (event.mouseWheel.delta > 0)
        {
            m_cursor.setType(m_cursor.getType() < Brick::UNBREAKABLE
                    ? static_cast<Brick::Type>(m_cursor.getType() + 1)
                    : Brick::START);
        }
        else
        {
            m_cursor.setType(m_cursor.getType() > Brick::START
                    ? static_cast<Brick::Type>(m_cursor.getType() - 1)
                    : Brick::UNBREAKABLE);
        }
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
    m_menu.onEvent(event);
}


void Editor::onFocus()
{
    m_level.loadAt(m_opt_levels->getSelectedValue());
    updateTexture();
}


void Editor::draw(sf::RenderTarget& target, sf::RenderStates) const
{
    target.draw(m_background);
    target.draw(m_borders_sprite);
    target.draw(m_level_sprite);
    target.draw(m_cursor);

    // Draw GUI menu
    m_menu.show();
}


void Editor::updateTexture()
{
    m_level_texture.clear(sf::Color(255, 255, 255, 0));

    // Draw bricks
    m_level_texture.draw(m_level);
    m_level_texture.draw(m_cursor_prelight, 4, sf::Quads);

    // Draw grid
    if (m_show_grid)
    {
        m_level_texture.draw(m_grid_lines, LevelManager::NB_BRICK_LINES * 2, sf::Lines);
        m_level_texture.draw(m_grid_cols, LevelManager::NB_BRICK_COLS * 2, sf::Lines);
    }

    m_level_texture.display();
}


void Editor::toggleGrid()
{
    m_show_grid = !m_show_grid;

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
