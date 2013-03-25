#ifndef GUI_LABEL_HPP
#define GUI_LABEL_HPP

#include "Widget.hpp"
#include "BitmapText.hpp"

namespace gui
{

class Label: public Widget
{
public:
	Label();
	Label(const sf::String& string);

	void setString(const sf::String& string);
	const sf::String& getString() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	BitmapText m_text;
};

}

#endif // GUI_LABEL_HPP
