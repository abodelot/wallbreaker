#include "Theme.hpp"

namespace gui
{


sf::Color Theme::textColor(sf::Color::White);
sf::Color Theme::backgroundColor(sf::Color::Blue);
sf::Color Theme::hoverColor(sf::Color(0x57, 0x30, 0x62));
sf::Color Theme::focusColor = sf::Color(0x57, 0x30, 0x62);

sf::Color Theme::topBorderColor(sf::Color::Black);
sf::Color Theme::bottomBorderColor(sf::Color::Black);

float Theme::borderSize(1.f);
float Theme::widgetWidth(86.f);
float Theme::padding(3.f);
float Theme::margin(7.f);

sf::Sound Theme::clickSound;

BitmapFont Theme::font;

float Theme::getBaseHeight()
{
    return font.getGlyphHeight() + padding * 2;
}

sf::Color hexToColor(const std::string& hexcolor)
{
    sf::Color color;
    if (hexcolor.size() == 7 && hexcolor[0] == '#')
    {
        color.r = strtoul(hexcolor.substr(1, 2).c_str(), NULL, 16);
        color.g = strtoul(hexcolor.substr(3, 2).c_str(), NULL, 16);
        color.b = strtoul(hexcolor.substr(5, 2).c_str(), NULL, 16);
    }
    return color;
}


sf::Color modulateColor(const sf::Color& source, float factor)
{
    sf::Color color(source);
    color.r = std::min<int>(source.r * factor, 255);
    color.g = std::min<int>(source.g * factor, 255);
    color.b = std::min<int>(source.b * factor, 255);
    return color;
}
}
