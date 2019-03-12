#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>
#include "Gui/BitmapText.hpp"

/**
 * Info panel
 */
class HUD
{
public:
    HUD();

    void setLevel(int level);

    void setBrickCount(int bricks);

    void setScore(int score);

    void setHighscore(int highscore);

    void setLiveCount(int n);

    /**
     * Get texture used internally for rendering
     */
    const sf::Texture& getTexture() const;

private:
    HUD(const HUD& hud);

    void updateTexture();

    struct Item: public sf::Drawable
    {
        Item();
        void setPosition(float x , float y);
        void setLabel(const sf::String& str);
        void setValue(int value);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        BitmapText label;
        BitmapText value;
    };

    Item              m_bricks;
    Item              m_level;
    Item              m_score;
    Item              m_highscore;
    int               m_life_count;
    BitmapText        m_life_label;
    sf::Sprite        m_life_icon;
    sf::RenderTexture m_render_texture;
};

#endif // HUD_HPP
