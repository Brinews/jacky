#ifndef GAMEBUILDER_H
#define GAMEBUILDER_H

#include "game.h"
#include "configbuilder.h"

#include <string>

/**
 * @brief The GameBuilder class
 * Builds a game
 * Can be extended to create more complex games
 */
class GameBuilder
{
    Game *game; // the game to be built

public:
    GameBuilder();
    // this destructor should not delete game
    // game is used by main method, and should be deleted by main method
    virtual ~GameBuilder() {}

    Game *getGame() { return game; }

    // sets size of the game window
    void setSize(int w, int h);
    // sets config for game
    void setConfig(std::string filename = CONFIG_FILE);
    // makes spaceship for game
    void makeShip();
    void makeAlien();
    // starts timer for game
    void startTimer(int tick);

protected:
    // creates a config using the builder
    virtual Config *createConfig(ConfigBuilder &builder);
};

#endif // GAMEBUILDER_H
