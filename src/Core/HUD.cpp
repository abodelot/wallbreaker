#include "HUD.hpp"
#include "Core/Effect.hpp"
#include "Core/Resources.hpp"
#include "Gui/Theme.hpp"


HUD::HUD():
    m_lifeCount(0),
    m_lifeLabel(gui::Theme::font)
{
    m_lifeLabel.setString("Lifes");
    m_lifeLabel.setPosition((HUD::WIDTH - m_lifeLabel.getSize().x) / 2, 20);

    // Life icons
    const sf::Texture& texture = Resources::getTexture("hud-life.png");
    const sf::Vector2u& iconSize = texture.getSize();
    // Center horizontally, 1px padding between icons
    float x = (HUD::WIDTH - (iconSize.x + 1) * MAX_PLAYER_LIVES) / 2 + iconSize.x / 2;
    float y = m_lifeLabel.getY() + 20;
    for (int i = 0; i < MAX_PLAYER_LIVES; ++i)
    {
        m_lifeIcons[i].setTexture(texture);
        m_lifeIcons[i].setPosition(x, y);
        m_lifeIcons[i].setOrigin(iconSize.x / 2, iconSize.y / 2);
        x += iconSize.x + 1;
    }

    m_level.setPosition(0, 60);
    m_level.setLabel("Level");

    m_bricks.setPosition(0, 100);
    m_bricks.setLabel("Bricks");

    m_score.setPosition(0, 140);
    m_score.setLabel("Score");

    m_highscore.setPosition(0, 180);
    m_highscore.setLabel("Highscore");
}


void HUD::setLevel(int level)
{
    m_level.setValue(level);
}


void HUD::setBrickCount(int bricks)
{
    m_bricks.setValue(bricks);
}


void HUD::setScore(int score)
{
    m_score.setValue(score);
}


void HUD::setHighscore(int highscore)
{
    m_highscore.setValue(highscore);
}


void HUD::setLifeCount(int lifeCount)
{
    if (lifeCount > 0)
    {
        Effect::zoomAndRevert(m_lifeIcons[lifeCount - 1], 2, 0.3);
    }

    for (int i = 0; i < MAX_PLAYER_LIVES; ++i)
    {
        m_lifeIcons[i].setColor(i < lifeCount ? sf::Color::White : sf::Color::Black);
    }
    m_lifeCount = lifeCount;
}


void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_level, states);
    target.draw(m_bricks, states);
    target.draw(m_score, states);
    target.draw(m_highscore, states);
    target.draw(m_lifeLabel, states);
    for (int i = 0; i < MAX_PLAYER_LIVES; ++i)
    {
        target.draw(m_lifeIcons[i], states);
    }
}

// HUD::Item -------------------------------------------------------------------

HUD::Item::Item():
    label(gui::Theme::font),
    value(gui::Theme::font)
{
    value.setColor(sf::Color::Yellow);
    setValue(0);
}


void HUD::Item::setPosition(float x, float y)
{
    label.setPosition(x, y);
}


void HUD::Item::setLabel(const sf::String& str)
{
    label.setString(str);
    label.setX((HUD::WIDTH - label.getSize().x) / 2);
}


void HUD::Item::setValue(int val)
{
    value.setString(std::to_string(val));
    value.setPosition(
        (HUD::WIDTH - value.getSize().x) / 2,
        label.getPosition().y + label.getSize().y + 2
    );
}


void HUD::Item::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(label, states);
    target.draw(value, states);
}
