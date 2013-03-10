#include "Game.hpp"
#include "Screens/MainMenu.hpp"

/*
Hi,

Here my last SFML project, It's a very simple breakout clone, although I took most of my inspiration from the superior Akanoid.

The game itself it not impressive in any way, but I wanted a base project for
experimenting new features, such as particle system, easing/animation utilities,
and bitmap fonts.

And it's always nice to actually finish a project for once.

[Windows release]
[Linux release]
Sources are available on the github repository, feel free to have a look.

*/


int main(int argc, char* argv[])
{
	Game& game = Game::getInstance();
	game.init(argv[0]);
	game.nextScreen(new MainMenu);
	game.run();
	return 0;
}
