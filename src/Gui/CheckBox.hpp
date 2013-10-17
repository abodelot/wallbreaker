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
	CheckBox(bool checked = false, int id = -1);

	bool isChecked() const;

	void check(bool checked);

	// callbacks ---------------------------------------------------------------

	void onStateChanged(State state);
	void onMouseReleased(float x, float y);
	void onKeyPressed(sf::Keyboard::Key key);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Box<Cross> m_box;
	bool       m_checked;
};

}

#endif // GUI_CHECKBOX_HPP
