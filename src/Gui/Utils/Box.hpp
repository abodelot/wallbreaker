#ifndef GUI_BOX_HPP
#define GUI_BOX_HPP

#include <SFML/Graphics.hpp>
#include "../Widget.hpp"

namespace gui
{

/**
 * Utility class used by widgets for holding visual components
 */
template <class T>
class Box: public sf::Drawable
{
public:
    Box();
    Box(const T& item);

    /**
     * Get box position
     */
    const sf::Vector2f& getPosition() const;

    void move(float dx, float dy);

    /**
     * Set the box dimensions
     */
    void setSize(float width, float height);

    /**
     * Get box dimensions
     */
    sf::Vector2f getSize() const;

    /**
     * Center the item inside the box
     */
    void centerItem();

    void setBodyColor(const sf::Color& color);

    void setTopLeftBorderColor(const sf::Color& color);

    void setBottomRightBorderColor(const sf::Color& color);

    /**
     * @return true if point is inside the box limits
     */
    bool containsPoint(const sf::Vector2f& pos) const;

    /**
     * Apply "hovered" effect
     */
    void prelight();

    /**
     * Apply "pressed" effect
     */
    void press();

    /**
     * Remove applied effect
     */
    void release();

    void applyState(State state);

    /**
     * Access to the encapsulated object
     */
    inline       T& item()       { return m_item; }
    inline const T& item() const { return m_item; }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    T          m_item;
    bool       m_pressed;
    sf::Vertex m_topLeftBorder[4];
    sf::Vertex m_bottomRightBorder[4];
    sf::Vertex m_background[4];
};

}

#include "Box.inl"

#endif // GUI_BOX_HPP
