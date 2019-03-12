#include "HUD.hpp"
#include "Core/Config.hpp"
#include "Core/Resources.hpp"
#include "Gui/Theme.hpp"

#define ROW_WIDTH 64

HUD::HUD():
    m_life_count(0),
    m_life_label(gui::Theme::getFont())
{
    m_render_texture.create(ROW_WIDTH, APP_HEIGHT);

    m_life_label.setString("Lifes");
    m_life_label.setPosition((ROW_WIDTH - m_life_label.getSize().x) / 2, 20);
    m_life_icon.setTexture(Resources::getTexture("hud-life.png"));

    m_level.setPosition(0, 60);
    m_level.setLabel("Level");

    m_bricks.setPosition(0, 100);
    m_bricks.setLabel("Bricks");

    m_score.setPosition(0, 140);
    m_score.setLabel("Score");

    m_highscore.setPosition(0, 180);
    m_highscore.setLabel("High\nscore");
}


void HUD::setLevel(int level)
{
    m_level.setValue(level);
    updateTexture();
}


void HUD::setBrickCount(int bricks)
{
    m_bricks.setValue(bricks);
    updateTexture();
}


void HUD::setScore(int score)
{
    m_score.setValue(score);
    updateTexture();
}


void HUD::setHighscore(int highscore)
{
    m_highscore.setValue(highscore);
    updateTexture();
}


void HUD::setLiveCount(int n)
{
    m_life_count = n;
    updateTexture();
}


const sf::Texture& HUD::getTexture() const
{
    return m_render_texture.getTexture();
}


void HUD::updateTexture()
{
    m_render_texture.clear(sf::Color::Transparent);
    m_render_texture.draw(m_level);
    m_render_texture.draw(m_bricks);
    m_render_texture.draw(m_score);
    m_render_texture.draw(m_highscore);

    m_render_texture.draw(m_life_label);

    float x = (ROW_WIDTH - m_life_icon.getTextureRect().width * m_life_count) / 2;
    for (int i = 0; i < m_life_count; ++i)
    {
        m_life_icon.setPosition(x + i * m_life_icon.getTextureRect().width, m_life_label.getY() + 14);
        m_render_texture.draw(m_life_icon);
    }

    m_render_texture.display();
}

// HUD::Item -------------------------------------------------------------------

HUD::Item::Item():
    label(gui::Theme::getFont()),
    value(gui::Theme::getFont())
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
    label.setX((ROW_WIDTH - label.getSize().x) / 2);
}


void HUD::Item::setValue(int val)
{
    value.setString(std::to_string(val));
    value.setPosition((ROW_WIDTH - value.getSize().x) / 2, label.getPosition().y + label.getSize().y + 2);
}


void HUD::Item::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(label, states);
    target.draw(value, states);
}
