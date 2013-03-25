#include "OptionsMenu.hpp"
#include "Core/Config.hpp"
#include "Core/Game.hpp"
#include "Core/SoundSystem.hpp"
#include "Gui/Button.hpp"


OptionsMenu::OptionsMenu():
	m_menu(Game::getInstance().getWindow())
{
	m_menu.setPosition(20, 100);

	// Create button for turning sound on/off
	m_but_sound = new gui::Button("Turn off");
	m_menu.add("Sound:", m_but_sound, 1);

	// Create options box for selecting a resolution
	m_opt_resolution = new gui::OptionsBox<sf::Vector2u>;
	addResolution({APP_WIDTH,     APP_HEIGHT});
	addResolution({APP_WIDTH * 2, APP_HEIGHT * 2});
	addResolution({APP_WIDTH * 3, APP_HEIGHT * 3});
	m_menu.add("Resolution:", m_opt_resolution, 2);

	m_menu.add("", new gui::Button("Back"), 3);
}


void OptionsMenu::onEvent(const sf::Event& event)
{
	switch (m_menu.onEvent(event))
	{
		case 1: // Sound
		{
			bool sound = !SoundSystem::isSoundEnabled();
			SoundSystem::enableSound(sound);
			if (sound)
			{
				SoundSystem::playSound("ball.ogg");
				m_but_sound->setString("Turn off");
			}
			else
			{
				m_but_sound->setString("Turn on");
			}
			break;
		}
		case 2: // Resolution
		{
			const sf::Vector2u& res = m_opt_resolution->getSelectedValue();
			Game::getInstance().setResolution(res.x, res.y);
			break;
		}
		case 3:
			Game::getInstance().previousScreen();
			break;

	}
}


void OptionsMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.clear({0x16, 0x1e, 0x26});
	m_menu.draw();
}


void OptionsMenu::addResolution(const sf::Vector2u& res)
{
	sf::Vector2u current = Game::getInstance().getWindow().getSize();
	m_opt_resolution->addItem(
		std::to_string(res.x) + "x" + std::to_string(res.y),
		res,
		res == current // Default selection is current resolution
	);
}
