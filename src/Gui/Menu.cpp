#include "Menu.hpp"
#include "Theme.hpp"

using namespace gui;


Menu::Menu(sf::RenderWindow& window):
    Layout(Layout::Vertical),
    m_window(window),
    m_cursorType(sf::Cursor::Arrow)
{
}


void Menu::onEvent(const sf::Event& event)
{
    switch (event.type)
    {
    case sf::Event::MouseMoved:
        onMouseMoved(convertMousePosition(event.mouseMove.x, event.mouseMove.y));
        break;

    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            onMousePressed(convertMousePosition(event.mouseButton.x, event.mouseButton.y));
        }
        break;

    case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            onMouseReleased(convertMousePosition(event.mouseButton.x, event.mouseButton.y));
        }
        break;

    case sf::Event::MouseWheelMoved:
        onMouseWheelMoved(event.mouseWheel.delta);
        break;

    case sf::Event::KeyPressed:
        onKeyPressed(event.key.code);
        break;

    case sf::Event::KeyReleased:
        onKeyReleased(event.key.code);
        break;

    case sf::Event::TextEntered:
        onTextEntered(event.text.unicode);
        break;

    default:
        break;
    }
}


sf::Vector2f Menu::convertMousePosition(int x, int y) const
{
    sf::Vector2f mouse = m_window.mapPixelToCoords(sf::Vector2i(x, y));
    mouse -= getPosition();
    return mouse;
}


void Menu::setCursor(sf::Cursor::Type cursorType)
{
    if (cursorType != m_cursorType)
    {
        gui::Theme::cursor.loadFromSystem(cursorType);
        m_window.setMouseCursor(gui::Theme::cursor);
        m_cursorType = cursorType;
    }
}
