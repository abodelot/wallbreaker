#ifndef GUI_CHECKBOX_HPP
#define GUI_CHECKBOX_HPP

#include "Widget.hpp"
#include "Utils/Box.hpp"
#include "Utils/Cross.hpp"

namespace gui
{

/**
 * A checkbox for enabling/disabling an option
 * Triggers an event when clicked
 */
class CheckBox: public Widget
{
public:
    CheckBox(bool checked = false);

    bool isChecked() const;

    void check(bool checked);

    // callbacks ---------------------------------------------------------------

    void onStateChanged(State state) override;
    void onMouseEnter() override;
    void onMouseLeave() override;
    void onMouseReleased(const sf::Vector2f& pos) override;
    void onKeyPressed(sf::Keyboard::Key key) override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    Box<Cross> m_box;
    bool m_checked;
};

}

#endif // GUI_CHECKBOX_HPP
