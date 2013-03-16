#include "HUD.hpp"
#include "Core/Config.hpp"
#include "Core/Resources.hpp"
#include "Gui/Theme.hpp"

#define ROW_WIDTH 90


HUD& HUD::getInstance()
{
	static HUD self;
	return self;
}


HUD::HUD():
	m_life_count(0),
	m_life_label(gui::Theme::getFont())
{
	m_render_texture.create(APP_WIDTH, 80);

	m_level.setLabel("Level:");
	m_level.setPosition(20, 10);
	m_score.setLabel("Score:");
	m_score.setPosition(130, 10);
	m_bricks.setLabel("Bricks:");
	m_bricks.setPosition(20, 25);
	m_highscore.setLabel("Highscore:");
	m_highscore.setPosition(130, 25);

	m_life_label.setString("Lifes:");
	m_life_label.setPosition(130, 40);
	m_life_icon.setTexture(Resources::getTexture("hud-life.png"));
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
	m_render_texture.draw(sf::Sprite(Resources::getTexture("hud-background.png")));
	m_render_texture.draw(m_level);
	m_render_texture.draw(m_bricks);
	m_render_texture.draw(m_score);
	m_render_texture.draw(m_highscore);

	m_render_texture.draw(m_life_label);

	sf::Vector2f pos = m_life_label.getPosition();
	for (int i = 1; i <= m_life_count; ++i)
	{
		m_life_icon.setPosition(pos.x + ROW_WIDTH - (m_life_icon.getTextureRect().width * i), pos.y);
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
	value.setPosition(x + ROW_WIDTH, y);
}


void HUD::Item::setLabel(const sf::String& str)
{
	label.setString(str);
}


void HUD::Item::setValue(int val)
{
	value.setString(std::to_string(val));
	value.setX(label.getX() + ROW_WIDTH - value.getSize().x);
}


void HUD::Item::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(label, states);
	target.draw(value, states);
}
