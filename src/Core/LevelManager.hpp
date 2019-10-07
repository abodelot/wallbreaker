#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <fstream>
#include "Config.hpp"
#include "Entities/Brick.hpp"

class LevelManager: public sf::Drawable, public sf::Transformable, sf::NonCopyable
{
public:
    static const int NB_BRICK_LINES = 29; // number of lines
    static const int NB_BRICK_COLS  = 13; // number of columns
    static const int BORDER_SIZE    = 8;  // size of the frame around the game scene

    static LevelManager& getInstance();

    /**
     * Open the file containing the levels
     */
    bool openFromFile(const std::string& filename);

    /**
     * Load the level located at index
     * @return number of bricks in current level
     */
    int loadAt(size_t index);

    /**
     * Reload the current level
     */
    int reload();

    /**
     * Load the previous level in levels file
     */
    int loadPrevious();

    /**
     * Load the next level in levels file
     */
    int loadNext();

    /**
     * Save current level to file
     */
    void save();

    /**
     * Create a new level (appended to levels file)
     * @return index of the level
     */
    size_t append();

    /**
     * Get index of the current level
     * @return level number (first is 1)
     */
    size_t getCurrentLevel() const;

    Brick& getBrick(int i, int j);

    /**
     * Get number of levels in the levels file
     */
    size_t getLevelCount() const;

    size_t getBrickCount() const;

private:
    LevelManager();
    ~LevelManager();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    /**
     * Load level at the current stream position
     */
    int load();

    Brick        m_bricks[NB_BRICK_LINES][NB_BRICK_COLS];
    std::fstream m_level_file;
    int          m_current_level;
    size_t       m_level_count;
    size_t       m_brick_count;
};

#endif // LEVEL_HPP
