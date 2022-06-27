#ifndef GUI_MENU_HPP
#define GUI_MENU_HPP

#include <SFML/Graphics.hpp>
#include "Layout.hpp"

namespace gui
{

class Menu: public gui::Layout
{
public:
    Menu(sf::RenderWindow& window);

    /**
     * Handle event and propagate it to widgets
     */
    void onEvent(const sf::Event& event);

private:
    /**
     * Get mouse cursor relative position
     * @param x: absolute x position from the event
     * @param y: absolute y position from the event
     * @return mouse position relative to menu position
     */
    sf::Vector2f convertMousePosition(int x, int y) const;

    void setCursor(sf::Cursor::Type cursorType) override;

    sf::RenderWindow& m_window;
    sf::Cursor::Type m_cursorType;
};

}

#endif // GUI_MENU_HPP
