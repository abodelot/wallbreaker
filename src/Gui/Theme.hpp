#ifndef GUI_THEME_HPP
#define GUI_THEME_HPP

#include "BitmapFont.hpp"
#include <SFML/Audio/Sound.hpp>
#include <map>
#include <string>

namespace gui
{

class Theme
{
public:
    static sf::Color textColor;
    static sf::Color backgroundColor;
    static sf::Color hoverColor;
    static sf::Color focusColor;
    static sf::Color topBorderColor;
    static sf::Color bottomBorderColor;

    static float borderSize;
    static float widgetWidth;
    static float padding; // Spacing inside widget
    static float margin;  // Spacing between widgets

    // Sound effect on button clicked
    static sf::Sound clickSound;

    // The GUI global font
    static BitmapFont font;

    static sf::Cursor cursor;

    /**
     * Widget height based on text height
     */
    static float getBaseHeight();
};

/**
 * Build color from hexadecimal string formatted as "#000000"
 */
sf::Color hexToColor(const std::string& string);

sf::Color modulateColor(const sf::Color& color, float factor);

}

#endif // GUI_THEME_HPP
