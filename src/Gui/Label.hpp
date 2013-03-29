#ifndef GUI_LABEL_HPP
#define GUI_LABEL_HPP

#include "Widget.hpp"
#include "BitmapText.hpp"

namespace gui
{

/**
 * Widget for displaying a simple text
 * Cannot trigger any event (no user interaction functionality is provided)
 */
class Label: public Widget
{
public:
	Label();
	Label(const sf::String& string);

	/**
	 * Set the label's text
	 */
	void setString(const sf::String& string);
	const sf::String& getString() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	BitmapText m_text;
};

}

#endif // GUI_LABEL_HPP
