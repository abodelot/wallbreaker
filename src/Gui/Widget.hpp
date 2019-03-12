#ifndef GUI_WIDGET_HPP
#define GUI_WIDGET_HPP

#include <SFML/Graphics.hpp>

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
     * Give an ID to the widget
     */
    void setID(int id);
    int getID() const;

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
     * Notify parent that the widget has been triggered by user input
     */
    virtual void triggerCallback(const Widget* triggered = NULL);

    void setState(State state);
    State getState() const;

    void transformStates(sf::RenderStates& states) const;

    /**
     * Set the widget's container (parent)
     */
    void setParent(Layout* parent);

    virtual Layout* toLayout() { return NULL; }

private:
    Layout*      m_parent;
    Widget*      m_previous;
    Widget*      m_next;

    State        m_state;
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    int          m_id;
    bool         m_selectable;
};

}

#endif // GUI_WIDGET_HPP
