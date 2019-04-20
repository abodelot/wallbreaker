#include "Game.hpp"
#include "Screens/Editor.hpp"
#include "Screens/MainMenu.hpp"
#include "Screens/OptionsMenu.hpp"
#include "Screens/Wallbreaker.hpp"


int main(int /*argc*/, char* argv[])
{
    Game& game = Game::getInstance();
    game.init(argv[0]);
    // Register game screens
    game.addScreen("Editor",      new Editor);
    game.addScreen("MainMenu",    new MainMenu);
    game.addScreen("OptionsMenu", new OptionsMenu);
    game.addScreen("Wallbreaker", new Wallbreaker);

    game.setCurrentScreen("MainMenu");
    game.run();
    return 0;
}
