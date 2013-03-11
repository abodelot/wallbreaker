#include "Game.hpp"
#include "Screens/MainMenu.hpp"


int main(int argc, char* argv[])
{
	Game& game = Game::getInstance();
	game.init(argv[0]);
	game.nextScreen(new MainMenu);
	game.run();
	return 0;
}
