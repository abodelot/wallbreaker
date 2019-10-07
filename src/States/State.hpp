#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Graphics.hpp>

/**
 * Abstract base class for game states
 * Each state keeps a pointer to the previous one, so we can go back in history
 */
class State: public sf::Drawable, sf::NonCopyable
{
public:
    State();

    virtual ~State() {}

    /**
     * Feed an event to the screen
     */
    virtual void onEvent(const sf::Event&) {}

    /**
     * Notify the screen it is the current one
     */
    virtual void onFocus() {}

    virtual void update(float) {}

    void setPrevious(State* state);

    State* getPrevious() const;

private:
    State* m_previous;
};

#endif // STATE_HPP
