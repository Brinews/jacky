#ifndef CONFIGBUILDER_H
#define CONFIGBUILDER_H

#include "config.h"

#include <string>
#include <vector>

/**
 * @brief The ConfigBuilder class
 * Builds a config given a configuration file
 * Can be extended to parse more complex files
 */
class ConfigBuilder
{
    Config *conf; // the config to be built

public:
    ConfigBuilder(std::string filename);
    // this destructor should not delete conf
    // conf is used by the game, and should be deleted by the game
    virtual ~ConfigBuilder() {}

    Config *getConfig() { return conf; }

    // adds spaceship properties from file to config
    void setShip();
    // adds spaceship actions from file to config
    void setActions();

    void setAlien();

    // a method to split a string into tokens, using a delimeter
    void split(const std::string &text, char delim, std::vector<std::string> &tokens) const;
};

#endif // CONFIGBUILDER_H
