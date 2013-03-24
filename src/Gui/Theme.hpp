#ifndef GUI_THEME_HPP
#define GUI_THEME_HPP

#include <map>
#include <string>
#include "BitmapFont.hpp"

namespace gui
{

class Theme
{
public:
	/**
	 * Load the font
	 */
	static void load(const std::string& path);

	/**
	 * Get font
	 */
	static const BitmapFont& getFont();

	/**
	 * Widget height based on text height
	 */
	static int getBaseLine();

	static sf::Color BG_COLOR;
	static sf::Color BG_COLOR_HOVER;
	static sf::Color BG_COLOR_PRESSED;
	static sf::Color BORDER_COLOR;

	static int WIDGET_WIDTH;
	static int PADDING; // Spacing inside widget
	static int MARGIN;  // Spacing between widgets

private:
	static BitmapFont m_font;
};

}

#endif // GUI_THEME_HPP
