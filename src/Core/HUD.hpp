#ifndef HUD_HPP
#define HUD_HPP

#include "Core/Config.hpp"
#include "Gui/BitmapText.hpp"
#include <SFML/Graphics.hpp>

/**
 * Info panel
 */
class HUD: public sf::Drawable, public sf::Transformable
{
public:
    static const int WIDTH = 64;

    HUD();

    void setLevel(int level);

    void setBrickCount(int bricks);

    void setScore(int score);

    void setHighscore(int highscore);

    void setLifeCount(int lifeCount);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    class Item: public sf::Drawable
    {
    public:
        Item();
        void setPosition(float x , float y);
        void setLabel(const sf::String& str);
        void setValue(int value);

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        BitmapText label;
        BitmapText value;
    };

    Item       m_bricks;
    Item       m_level;
    Item       m_score;
    Item       m_highscore;
    int        m_lifeCount;
    BitmapText m_lifeLabel;
    sf::Sprite m_lifeIcons[MAX_PLAYER_LIVES];
};

#endif // HUD_HPP
