#include <cstdio>
#include "BitmapFont.hpp"


BitmapFont::BitmapFont():
    m_width(0),
    m_height(0),
    m_glyphWidth(0),
    m_glyphHeight(0)
{
}


bool BitmapFont::loadFromFile(const std::string& image_path, int width, int height)
{
    if (m_texture.loadFromFile(image_path))
    {
        m_width = width;
        m_height = height;

        // Compute character area from image dimensions
        m_glyphWidth = m_texture.getSize().x / m_width;
        m_glyphHeight = m_texture.getSize().y / m_height;
        return true;
    }
    return false;
}


const sf::Texture& BitmapFont::getTexture() const
{
    return m_texture;
}


sf::IntRect BitmapFont::getGlyphRect(char character) const
{
    if (character < FIRST_CHAR || character > LAST_CHAR)
    {
        fprintf(stderr, "warning: non printable character (ASCII %d\n", character);
        character = '?';
    }
    character -= FIRST_CHAR;

    sf::IntRect subrect;
    subrect.left = (character % m_width) * m_glyphWidth;
    subrect.width = m_glyphWidth;
    subrect.top = (character / m_width) * m_glyphHeight;
    subrect.height = m_glyphHeight;
    return subrect;
}


int BitmapFont::getGlyphHeight() const
{
    return m_glyphHeight;
}


int BitmapFont::getGlyphWidth() const
{
    return m_glyphWidth;
}
