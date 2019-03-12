#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SFML/Graphics.hpp>

/**
 * Abstract base class for screens
 */
class Screen: public sf::Drawable, sf::NonCopyable
{
public:
    Screen();

    virtual ~Screen() {}

    /**
     * Feed an event to the screen
     */
    virtual void onEvent(const sf::Event&) {}

    /**
     * Notify the screen it is the current one
     */
    virtual void onFocus() {}

    virtual void update(float) {}

    void setPrevious(Screen* screen);

    Screen* getPrevious() const;

private:
    Screen* m_previous;
};

#endif // SCREEN_HPP
