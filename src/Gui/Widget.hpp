#ifndef GUI_WIDGET_HPP
#define GUI_WIDGET_HPP

#include <SFML/Graphics.hpp>
#include <functional>

namespace gui
{

enum State
{
    StateDefault,
    StateHovered,
    StateFocused,
};

class Layout;

/**
 * Abstract base class for gui widgets
 */
class Widget: public sf::Drawable
{
public:
    Widget();

    /**
     * Widget's position
     */
    void setPosition(const sf::Vector2f& pos);
    void setPosition(float x, float y);
    const sf::Vector2f& getPosition() const;

    /**
     * Widget's dimensions
     */
    void setSize(const sf::Vector2f& size);
    void setSize(float widget, float height);
    const sf::Vector2f& getSize() const;

    /**
     * Check if a point is inside the widget
     */
    bool containsPoint(const sf::Vector2f& point) const;

    /**
     * Check if the widget can be selected and trigger events
     */
    bool isSelectable() const;

    /**
     * Check if the widget is currently focused
     */
    bool isFocused() const;

    /**
     * Define a user callback function for this widget
     */
    void setCallback(std::function<void(void)> callback);

    // Callbacks ---------------------------------------------------------------

    virtual void onStateChanged(State state);
    virtual void onMouseMoved(const sf::Vector2f&);
    virtual void onMousePressed(const sf::Vector2f&);
    virtual void onMouseReleased(const sf::Vector2f&);
    virtual void onMouseWheelMoved(int delta);
    virtual void onKeyPressed(sf::Keyboard::Key key);
    virtual void onKeyReleased(sf::Keyboard::Key key);
    virtual void onTextEntered(sf::Uint32 unicode);

protected:
    friend class Layout;

    void setSelectable(bool selectable);

    /**
     * Call the user callback function associated to this widget, if any
     */
    void triggerCallback();

    void setState(State state);
    State getState() const;

    void transformStates(sf::RenderStates& states) const;

    /**
     * Set the widget's container (parent)
     */
    void setParent(Layout* parent);

    virtual Layout* toLayout() { return nullptr; }

private:
    Layout*      m_parent;
    Widget*      m_previous;
    Widget*      m_next;
    State        m_state;
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    bool         m_selectable;

    std::function<void(void)> m_callback;
};

}

#endif // GUI_WIDGET_HPP
