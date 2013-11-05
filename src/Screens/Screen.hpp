#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SFML/Graphics.hpp>

/**
 * Abstract base class for screens
 */
class Screen: public sf::Drawable, sf::NonCopyable
{
public:
	virtual ~Screen() {}

	virtual void onEvent(const sf::Event& event) {}

	virtual void onFocus() {}

	virtual void update(float frametime) {}
};

#endif // SCREEN_HPP
