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
	 * Load the GUI global font
	 */
	static bool loadFont(const std::string& path);

	/**
	 * Get font
	 */
	static const BitmapFont& getFont();

	/**
	 * Widget height based on text height
	 */
	static float getBaseLine();

	static sf::Color BG_COLOR;
	static sf::Color BG_COLOR_HOVER;
	static sf::Color BG_COLOR_FOCUS;
	static sf::Color BORDER_COLOR;
	static sf::Color TEXT_COLOR;

	static float BORDER_SIZE;
	static float WIDGET_WIDTH;
	static float PADDING; // Spacing inside widget
	static float MARGIN;  // Spacing between widgets
	static sf::Keyboard::Key PREV_WIDGET_KEY;
	static sf::Keyboard::Key NEXT_WIDGET_KEY;

private:
	static BitmapFont m_font;
};

}

#endif // GUI_THEME_HPP
