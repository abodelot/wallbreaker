#include "OptionsMenu.hpp"
#include "Core/Config.hpp"
#include "Core/Game.hpp"
#include "Core/SoundSystem.hpp"
#include "Core/Resources.hpp"
#include "Gui/Button.hpp"
#include "Gui/Label.hpp"


OptionsMenu::OptionsMenu():
	m_background(Resources::getTexture("background.png")),
	m_menu(Game::getInstance().getWindow())
{
	m_menu.setPosition(40, 40);
	m_menu.addLabel("Options")->setCharacterSize(2);

	// Create checkbox for turning music on/off
	m_ck_music = new gui::CheckBox(SoundSystem::isMusicEnabled());

	// Create checkbox for turning sound on/off
	m_ck_sound = new gui::CheckBox(SoundSystem::isSoundEnabled());

	// Create options box for selecting a resolution
	m_opt_resolution = new gui::OptionsBox<sf::Vector2u>;
	addResolution({APP_WIDTH,     APP_HEIGHT});
	addResolution({APP_WIDTH * 2, APP_HEIGHT * 2});
	addResolution({APP_WIDTH * 3, APP_HEIGHT * 3});

	// Add widgets in form
	gui::Layout* form = m_menu.addLayout(gui::Layout::Form);
	form->addRow("Music:", m_ck_music, 1);
	form->addRow("Sound:", m_ck_sound, 2);
	form->addRow("Resolution:", m_opt_resolution, 3);
	form->addRow("", new gui::Button("Back"), 4);
}


void OptionsMenu::onEvent(const sf::Event& event)
{
	switch (m_menu.onEvent(event))
	{
		case 1: // Music on/off
			SoundSystem::enableMusic(m_ck_music->isChecked());
			break;
		case 2: // Sound on/off
			SoundSystem::enableSound(m_ck_sound->isChecked());
			if (m_ck_sound->isChecked())
			{
				SoundSystem::playSound("ball.ogg");
			}
			break;
		case 3: // Resolution
		{
			const sf::Vector2u& res = m_opt_resolution->getSelectedValue();
			Game::getInstance().setResolution(res.x, res.y);
			break;
		}
		case 4: // Back
			Game::getInstance().restorePreviousScreen();
			break;

	}
}


void OptionsMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background, states);
	m_menu.show();
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
