#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "State.hpp"
#include "Core/Config.hpp"
#include "Core/LevelManager.hpp"
#include "Entities/Brick.hpp"
#include "Gui/Menu.hpp"

namespace gui
{
template <class T>
class OptionsBox;
class CheckBox;
class Image;
}

class Editor: public State
{
public:
    Editor();

    void onEvent(const sf::Event& event) override;

    void onFocus() override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    /**
     * Update internal texture used for rendering current level
     */
    void updateTexture();

    /**
     * Turn the visual grid on/off
     */
    void toggleGrid();

    /**
     * Put or remove a brick at grid index
     */
    void putBrick(const sf::Vector2i& index);

    void setCursorBrick(Brick::Type type);

    static constexpr int m_width = LevelManager::NB_BRICK_COLS * Brick::WIDTH;
    static constexpr int m_height = LevelManager::NB_BRICK_LINES * Brick::HEIGHT;

    sf::Sprite        m_background;
    LevelManager&     m_level;
    Brick             m_cursor;
    bool              m_cursorOnBrick;
    sf::Vector2i      m_cursorCoords;

    sf::RenderTexture m_levelTexture;
    sf::Sprite        m_levelSprite;
    sf::Sprite        m_bordersSprite;

    // Grid: 2 vertices per line
    sf::Vertex m_gridCols[LevelManager::NB_BRICK_COLS * 2];
    sf::Vertex m_gridLines[LevelManager::NB_BRICK_LINES * 2];

    // GUI elements
    gui::Menu m_menu;
    gui::OptionsBox<size_t>* m_optLevels;
    gui::CheckBox* m_ckGrid;
    gui::Image* m_imgBrick;
};

#endif // EDITOR_HPP
