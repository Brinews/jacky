#include "gamebuilder.h"

#include <iostream>

GameBuilder::GameBuilder()
{
    game = new Game();
}

void GameBuilder::setSize(int w, int h)
{
    game->resize(w, h);
}

Config *GameBuilder::createConfig(ConfigBuilder &builder)
{
    // set spaceship properties in config
    builder.setShip();
    // set spaceship actions in config
    builder.setActions();

    builder.setAlien();

    return builder.getConfig();
}

void GameBuilder::setConfig(std::string filename)
{
    try {
        // make ConfigBuilder use given configuration file
        ConfigBuilder builder(filename);
        Config *conf = createConfig(builder);
        game->setConf(conf);
    } catch (...) {
        // any exception thrown from before is caught here
        std::cout << "failed to set game config!" << std::endl;
        exit(1);
    }
}


void GameBuilder::makeAlien()
{
    game->setAlien(
        new Alien(game->getConf()->getShape(), 
            game->getConf()->getTInter()));
}


void GameBuilder::makeShip()
{
    // make spaceship using properties specified in config
    game->setShip(
        new Spaceship(game->getConf()->getSize(), game->getConf()->getXPos())
    );
}

void GameBuilder::startTimer(int tick)
{
    game->startTimer(tick);
}
