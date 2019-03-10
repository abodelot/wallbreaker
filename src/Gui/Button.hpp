#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP

#include "Widget.hpp"
#include "BitmapText.hpp"
#include "Utils/Box.hpp"

namespace gui
{

/**
 * A simple press button
 * Triggers an event when clicked
 */
class Button: public Widget
{
public:
    Button(const sf::String& string);

    /**
     * Set the displayed button label
     */
    void setString(const sf::String& string);
    const sf::String& getString() const;

    // callbacks ---------------------------------------------------------------

    void onStateChanged(State state) override;
    void onMouseMoved(const sf::Vector2f& pos) override;
    void onMousePressed(const sf::Vector2f& pos) override;
    void onMouseReleased(const sf::Vector2f& pos) override;
    void onKeyPressed(sf::Keyboard::Key key) override;
    void onKeyReleased(sf::Keyboard::Key key) override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    Box<BitmapText> m_box;
};

}

#endif // GUI_BUTTON_HPP
