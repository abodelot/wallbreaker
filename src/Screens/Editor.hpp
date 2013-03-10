#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "Screen.hpp"

class Editor: public Screen
{
public:

private:
	// override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // EDITOR_HPP
