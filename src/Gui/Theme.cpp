#include "Theme.hpp"

using namespace gui;

BitmapFont Theme::m_font;


void Theme::load(const std::string& name)
{
	m_font.loadFromFile(name);
}


const BitmapFont& Theme::getFont()
{
	return m_font;
}
