#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <string>
#include <map>

/**
 * Static class for loading and managing resources
 */
class Resources
{
public:
    /// Set path where resources are located
    static void setSearchPath(const std::string& path);

    /// Get a texture (load if not loaded yet)
    static sf::Texture& getTexture(const std::string& name);

    /// Get a font (load if not loaded yet)
    static sf::Font& getFont(const std::string& name);

    /// Get a sound buffer (load if not loaded yet)
    static sf::SoundBuffer& getSoundBuffer(const std::string& name);

private:
    static std::string m_path;

    typedef std::map<std::string, sf::Texture> TextureMap;
    static TextureMap m_textures;

    typedef std::map<std::string, sf::Font> FontMap;
    static FontMap m_fonts;

    typedef std::map<std::string, sf::SoundBuffer> SoundMap;
    static SoundMap m_sounds;
};

#endif // RESOURCES_HPP
