#ifndef BITMAP_FONT_HPP
#define BITMAP_FONT_HPP

#include <SFML/Graphics.hpp>

/*
TODO: create bitmap fonts with different character sizes
*/

class BitmapFont
{
public:
	// ASCII Printable characters range
	static const int FIRST_CHAR = 32;
	static const int LAST_CHAR = 126;

	BitmapFont();

	/**
	 * @param image_path: path to the bitmap font's image
	 * @param width: nb characters by line
	 * @param height: nb characters by column
	 */
	bool loadFromFile(const std::string& image_path, int width=10, int height=10);

	// Get the font's texture
	const sf::Texture& getTexture() const;

	/**
	 * Get a glyph texture rect
	 * @param char: glyph's letter
	 * @return texture sub rect
	 */
	sf::IntRect getGlyphRect(char character) const;

	int getGlyphHeight() const;

private:
	sf::Texture m_texture;
	int         m_width;
	int         m_height;
	int         m_glyph_width;
	int         m_glyph_height;
};


#endif // BITMAP_FONT_HPP
