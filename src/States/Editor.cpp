#include "Editor.hpp"
#include "Core/Game.hpp"
#include "Core/Resources.hpp"
#include "Gui/Button.hpp"
#include "Gui/CheckBox.hpp"
#include "Gui/Image.hpp"
#include "Gui/Label.hpp"
#include "Gui/OptionsBox.hpp"
#include <iostream>


Editor::Editor():
    m_background(Resources::getTexture("background.png")),
    m_level(LevelManager::getInstance()),
    m_cursorOnBrick(false),
    m_menu(Game::getInstance().getWindow())
{
    // Initialize render texture
    m_levelTexture.create(m_width, m_height);

    // Sprites used for render textures
    m_levelSprite.setTexture(m_levelTexture.getTexture());
    m_levelSprite.setPosition(LevelManager::BORDER_SIZE, LevelManager::BORDER_SIZE);

    m_bordersSprite.setTexture(Resources::getTexture("borders.png"));

    // Initialize visual grid (+0.5 for portable pixel-perfect rendition)
    const sf::Color gridColor(0xa0, 0x90, 0xc0);
    for (int i = 1; i < LevelManager::NB_BRICK_LINES; ++i)
    {
        m_gridLines[i * 2].color = gridColor;
        m_gridLines[i * 2].position = sf::Vector2f(0.5, Brick::HEIGHT * i - 0.5);
        m_gridLines[i * 2 + 1].color = gridColor;
        m_gridLines[i * 2 + 1].position = sf::Vector2f(m_width + 0.5, Brick::HEIGHT * i - 0.5);
    }

    for (int i = 1; i < LevelManager::NB_BRICK_COLS; ++i)
    {
        m_gridCols[i * 2].color = gridColor;
        m_gridCols[i * 2].position = sf::Vector2f(Brick::WIDTH * i - 0.5, 0.5);
        m_gridCols[i * 2 + 1].color = gridColor;
        m_gridCols[i * 2 + 1].position = sf::Vector2f(Brick::WIDTH * i - 0.5, m_height + 0.5);
    }

    m_cursor.setType(Brick::START);
    m_cursor.setColor(sf::Color(255, 255, 255, 128));

    // Create GUI menu
    m_menu.setPosition(m_width + LevelManager::BORDER_SIZE * 2 + 4, LevelManager::BORDER_SIZE);
    m_menu.addLabel(" Level Editor");

    // Populate level list
    m_optLevels = new gui::OptionsBox<size_t>;
    for (size_t i = 1; i <= m_level.getLevelCount(); ++i)
        m_optLevels->addItem("Level " + std::to_string(i), i);

    m_optLevels->setCallback([this]() {
        size_t level = m_optLevels->getSelectedValue();
        if (m_level.loadAt(level))
        {
            updateTexture();
        }
    });
    m_menu.add(m_optLevels);
    m_menu.addButton("Save", [this]() { m_level.save(); });
    m_menu.addButton("Reload", [this]() {
        m_level.reload();
        updateTexture();
    });
    m_menu.addButton("New level", [this]() {
        size_t index = m_level.append();
        // Insert new level in level list
        m_optLevels->addItem("Level " + std::to_string(index), index, true);
        updateTexture();
    });

    // Toggle the editor grid on/off
    m_ckGrid = new gui::CheckBox(false);
    m_ckGrid->setCallback([this]() { toggleGrid(); });
    gui::Layout* form = m_menu.addLayout(gui::Layout::Form);
    form->addRow("Show grid:", m_ckGrid);

    // Display current brick, select next on click
    m_imgBrick = new gui::Image();
    m_imgBrick->setTexture(*m_cursor.getTexture(), m_cursor.getTextureRect());
    m_imgBrick->setCallback([this]() {
        setCursorBrick(m_cursor.getType() < Brick::UNBREAKABLE
                ? static_cast<Brick::Type>(m_cursor.getType() + 1)
                : Brick::START);
    });
    form->addRow("Brick:", m_imgBrick);
    m_menu.addButton("Back", []() { Game::getInstance().restorePreviousState(); });

    updateTexture();
}


void Editor::onEvent(const sf::Event& event)
{
    switch (event.type)
    {
    case sf::Event::MouseMoved:
    {
        m_cursorOnBrick = false;

        // Convert mouse position and update cursor coords
        sf::Vector2f mouse = Game::getInstance().getWindow().mapPixelToCoords(
            {event.mouseMove.x, event.mouseMove.y});
        mouse.x -= LevelManager::BORDER_SIZE;
        mouse.y -= LevelManager::BORDER_SIZE;

        // Find brick index at the mouse position
        const sf::Vector2i coords(
            static_cast<int>(mouse.x) / Brick::WIDTH,
            static_cast<int>(mouse.y) / Brick::HEIGHT);
        if (mouse.x >= 0 && mouse.y >= 0
            && coords.x >= 0 && coords.x < LevelManager::NB_BRICK_COLS
            && coords.y >= 0 && coords.y < LevelManager::NB_BRICK_LINES)
        {
            if (coords != m_cursorCoords)
            {
                // Update brick cursor position
                m_cursor.setPosition(
                    LevelManager::BORDER_SIZE + coords.x * Brick::WIDTH,
                    LevelManager::BORDER_SIZE + coords.y * Brick::HEIGHT);
                m_cursorCoords = coords;
                putBrick(coords);
            }
            m_cursorOnBrick = true;
        }
        break;
    }
    case sf::Event::MouseButtonPressed:
        if (m_cursorCoords.x >= 0 && m_cursorCoords.x < LevelManager::NB_BRICK_COLS
            && m_cursorCoords.y >= 0 && m_cursorCoords.y < LevelManager::NB_BRICK_LINES)
        {
            putBrick(m_cursorCoords);
            updateTexture();
        }
        break;

    case sf::Event::MouseWheelMoved:
        if (event.mouseWheel.delta > 0)
        {
            setCursorBrick(m_cursor.getType() < Brick::UNBREAKABLE
                    ? static_cast<Brick::Type>(m_cursor.getType() + 1)
                    : Brick::START);
        }
        else
        {
            setCursorBrick(m_cursor.getType() > Brick::START
                    ? static_cast<Brick::Type>(m_cursor.getType() - 1)
                    : Brick::UNBREAKABLE);
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
    m_level.loadAt(m_optLevels->getSelectedValue());
    updateTexture();
}


void Editor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_background, states);
    target.draw(m_bordersSprite, states);
    target.draw(m_levelSprite, states);
    if (m_cursorOnBrick)
        target.draw(m_cursor, states);

    // Draw GUI menu
    target.draw(m_menu, states);
}


void Editor::updateTexture()
{
    m_levelTexture.clear(sf::Color(255, 255, 255, 16));

    // Draw bricks
    m_levelTexture.draw(m_level);

    // Draw grid
    if (m_ckGrid->isChecked())
    {
        m_levelTexture.draw(m_gridLines, LevelManager::NB_BRICK_LINES * 2, sf::Lines);
        m_levelTexture.draw(m_gridCols, LevelManager::NB_BRICK_COLS * 2, sf::Lines);
    }

    m_levelTexture.display();
}


void Editor::toggleGrid()
{
    updateTexture();
}


void Editor::putBrick(const sf::Vector2i& index)
{
    if (!m_cursorOnBrick)
        return;

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
        m_cursor.playSound();
    }
}


void Editor::setCursorBrick(Brick::Type type)
{
    m_cursor.setType(type);
    m_cursor.setColor(sf::Color(255, 255, 255, 128));
    m_imgBrick->setTexture(*m_cursor.getTexture(), m_cursor.getTextureRect());
}
