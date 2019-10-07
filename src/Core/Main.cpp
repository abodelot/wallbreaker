#include "Game.hpp"
#include "States/Editor.hpp"
#include "States/MainMenu.hpp"
#include "States/OptionsMenu.hpp"
#include "States/Wallbreaker.hpp"


int main(int /*argc*/, char* argv[])
{
    Game& game = Game::getInstance();
    game.init(argv[0]);
    // Register game states
    game.addState("Editor",      new Editor);
    game.addState("MainMenu",    new MainMenu);
    game.addState("OptionsMenu", new OptionsMenu);
    game.addState("Wallbreaker", new Wallbreaker);

    game.setCurrentState("MainMenu");
    game.run();
    return 0;
}
