#ifndef BITMAP_TEXT_HPP
#define BITMAP_TEXT_HPP

#include <SFML/Graphics.hpp>

#include "BitmapFont.hpp"


class BitmapText: public sf::Drawable, public sf::Transformable
{
public:
	BitmapText(const BitmapFont& font);

	/// Set the text's string
	void setString(const sf::String& string);

	/// Get the text's string
	const sf::String& getString() const;

	/// Set the text's bitmap font
	void setFont(const BitmapFont& font);

	/// Get the text's bitmap font
	const BitmapFont& getFont() const;

	/// Set the text's color
	void setColor(const sf::Color& color);

	/// Get the text's color
	const sf::Color& getColor() const;

	/// Clear the string content
	void clear();

	/// Get the characters length
	size_t getLength() const;

	sf::FloatRect getLocalBounds() const { return m_bounds; }

	void setX(float x);
	void setY(float y);
	float getX() const;
	float getY() const;
	/// Get the text dimensions
	sf::Vector2f getSize() const;

private:
	// override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void updateGeometry();

	const BitmapFont* m_font;
	sf::VertexArray   m_vertices;
	sf::String        m_string;
	sf::Color         m_color;
	sf::FloatRect     m_bounds;
};


#endif // BITMAP_TEXT_HPP

