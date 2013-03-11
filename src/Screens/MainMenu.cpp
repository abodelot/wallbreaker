#include "MainMenu.hpp"
#include "Core/Game.hpp"
#include "Core/Resources.hpp"
#include "Gui/Theme.hpp"

#include "Wallbreaker.hpp"
#include "Editor.hpp"

#define APP_AUTHOR   "Alexandre Bodelot"
#define APP_NAME     "Wallbreaker"
#define APP_VERSION  "0.2"

#define ABOUT_TEXT (APP_NAME " v" APP_VERSION " - Author: " APP_AUTHOR " - Powered by SFML")

MainMenu::MainMenu():
	m_about_text(gui::Theme::getFont()),
	m_menu(Game::getInstance().getWindow())
{
	const sf::Texture& t = Resources::getTexture("title.png");
	m_title.setTexture(t);
	m_title.setPosition(0, 16);


	//m_about_text.setColor(sf::Color::White);
	//m_about_text.setCharacterSize(10);
	m_about_text.setString(ABOUT_TEXT);
	m_about_text.setPosition(0, APP_HEIGHT - m_about_text.getSize().y);
//	m_view = sf::View(sf::FloatRect(0, 0, 640, 480));

	m_menu.setPosition(80, 80);
	m_menu.addButton("New game", 1);
	m_menu.addButton("Editor", 2);
	m_menu.addButton("Quit",  3);
}


void MainMenu::onEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		Game::getInstance().nextScreen(new Wallbreaker);
	}
	else
	{
		int id;
		if (m_menu.onEvent(event, id))
		{
			if (id == 1)
				Game::getInstance().nextScreen(new Wallbreaker);
			if (id == 2)
				Game::getInstance().nextScreen(new Editor);
			else if (id == 3)
				Game::getInstance().quit();
		}
	}
}


void MainMenu::onFocus()
{
	Game::getInstance().getWindow().setMouseCursorVisible(true);
}


void MainMenu::update(float frametime)
{
	m_about_text.move(-50 * frametime, 0);
	float width = m_about_text.getSize().x;
	if (m_about_text.getPosition().x + width < 0)
	{
		m_about_text.setPosition(APP_WIDTH, m_about_text.getPosition().y);
	}
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//target.setView(m_view);
	target.clear({0x16, 0x1e, 0x26});
	target.draw(m_title);

	target.draw(m_about_text);

	m_menu.draw();
}




