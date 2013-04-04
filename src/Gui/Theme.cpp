#include "Theme.hpp"

using namespace gui;

sf::Color Theme::BG_COLOR         = sf::Color::Blue;
sf::Color Theme::BG_COLOR_HOVER   = sf::Color(64, 128, 255);
sf::Color Theme::BG_COLOR_FOCUS   = sf::Color(0, 0, 128);
sf::Color Theme::BORDER_COLOR     = sf::Color::Cyan;

float     Theme::BORDER_SIZE      = 1.f;
float     Theme::WIDGET_WIDTH     = 86.f;
float     Theme::PADDING          = 2.f;
float     Theme::MARGIN           = 7.f;

sf::Keyboard::Key Theme::NEXT_WIDGET_KEY = sf::Keyboard::Down;
sf::Keyboard::Key Theme::PREV_WIDGET_KEY = sf::Keyboard::Up;

BitmapFont Theme::m_font;


void Theme::load(const std::string& name)
{
	m_font.loadFromFile(name);
}


const BitmapFont& Theme::getFont()
{
	return m_font;
}


float Theme::getBaseLine()
{
	return m_font.getGlyphHeight() + PADDING * 2;
}
