#include "Theme.hpp"

using namespace gui;

sf::Color Theme::BG_COLOR         = sf::Color(0x33, 0x33, 0x80);
sf::Color Theme::BG_COLOR_HOVER   = sf::Color(0x57, 0x30, 0x62);
sf::Color Theme::BG_COLOR_FOCUS   = sf::Color(0x1e, 0x1e, 0x48);
sf::Color Theme::BORDER_COLOR     = sf::Color(0xab, 0x5f, 0x41);
sf::Color Theme::TEXT_COLOR       = sf::Color::White;

float     Theme::BORDER_SIZE      = 1.f;
float     Theme::WIDGET_WIDTH     = 86.f;
float     Theme::PADDING          = 2.f;
float     Theme::MARGIN           = 7.f;

sf::Keyboard::Key Theme::NEXT_WIDGET_KEY = sf::Keyboard::Down;
sf::Keyboard::Key Theme::PREV_WIDGET_KEY = sf::Keyboard::Up;

BitmapFont Theme::m_font;


bool Theme::loadFont(const std::string& name)
{
	return m_font.loadFromFile(name);
}


const BitmapFont& Theme::getFont()
{
	return m_font;
}


float Theme::getBaseLine()
{
	return m_font.getGlyphHeight() + PADDING * 2;
}
