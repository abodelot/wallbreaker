#include "OptionsMenu.hpp"
#include "Core/Config.hpp"
#include "Core/Game.hpp"
#include "Core/SoundSystem.hpp"
#include "Gui/Button.hpp"
#include "Gui/Label.hpp"

OptionsMenu::OptionsMenu():
	m_menu(Game::getInstance().getWindow())
{
	m_menu.setPosition(40, 40);

	m_menu.add(new gui::Label("Options"))->setScale(2, 2);
	sf::Vector2f pos = m_menu.add(new gui::Label("Sound:"))->getPosition();
	m_menu.add(new gui::Label("Resolution:"));

	// Create checkbox for turning sound on/off
	m_ck_sound = new gui::CheckBox(SoundSystem::isSoundEnabled());
	m_menu.add(m_ck_sound, 1);
	pos.x += gui::Theme::WIDGET_WIDTH;
	m_ck_sound->setPosition(pos);


	// Create options box for selecting a resolution
	m_opt_resolution = new gui::OptionsBox<sf::Vector2u>;
	addResolution({APP_WIDTH,     APP_HEIGHT});
	addResolution({APP_WIDTH * 2, APP_HEIGHT * 2});
	addResolution({APP_WIDTH * 3, APP_HEIGHT * 3});
	m_menu.add(m_opt_resolution, 2);

	m_menu.addButton("Back", 3);
}


void OptionsMenu::onEvent(const sf::Event& event)
{
	switch (m_menu.onEvent(event))
	{
		case 1: // Sound on/off
		{
			SoundSystem::enableSound(m_ck_sound->isChecked());
			if (m_ck_sound->isChecked())
			{
				SoundSystem::playSound("ball.ogg");
			}
			break;
		}
		case 2: // Resolution
		{
			const sf::Vector2u& res = m_opt_resolution->getSelectedValue();
			Game::getInstance().setResolution(res.x, res.y);
			break;
		}
		case 3: // Back
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
