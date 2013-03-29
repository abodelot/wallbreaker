#include "Resources.hpp"


std::string           Resources::m_path = "./";
Resources::TextureMap Resources::m_textures;
Resources::FontMap    Resources::m_fonts;
Resources::SoundMap   Resources::m_sounds;


void Resources::setSearchPath(const std::string& path)
{
	if (!path.empty())
	{
		m_path = path;
	}
}


sf::Texture& Resources::getTexture(const std::string& name)
{
	TextureMap::iterator it = m_textures.find(name);
	if (it == m_textures.end())
	{
		sf::Texture& texture = m_textures[name];
		texture.loadFromFile(m_path + "images/" + name);
		return texture;
	}
	return it->second;
}


sf::Font& Resources::getFont(const std::string& name)
{
	FontMap::iterator it = m_fonts.find(name);
	if (it == m_fonts.end())
	{
		sf::Font& font = m_fonts[name];
		font.loadFromFile(m_path + "fonts/" + name);
		return font;
	}
	return it->second;
}


sf::SoundBuffer& Resources::getSoundBuffer(const std::string& name)
{
	SoundMap::iterator it = m_sounds.find(name);
	if (it == m_sounds.end())
	{
		sf::SoundBuffer& sound = m_sounds[name];
		sound.loadFromFile(m_path + "sounds/" + name);
		return sound;
	}
	return it->second;
}

