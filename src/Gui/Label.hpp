#ifndef GUI_LABEL_HPP
#define GUI_LABEL_HPP

#include "Widget.hpp"
#include "BitmapText.hpp"

namespace gui
{

/**
 * Widget for displaying a simple text
 * Cannot trigger any event (no user interaction functionality is provided)
 */
class Label: public Widget
{
public:
    Label();
    Label(const sf::String& string);

    /**
     * Label's text
     */
    void setText(const sf::String& string);
    const sf::String& getText() const;

    /**
     * Label's color
     */
    void setColor(const sf::Color& color);
    const sf::Color& getColor() const;

    void setCharacterSize(int size);
    int getCharacterSize() const;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    BitmapText m_text;
};

}

#endif // GUI_LABEL_HPP
