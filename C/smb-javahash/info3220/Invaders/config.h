#ifndef CONFIG_H
#define CONFIG_H

#include "iofile.h"

#include <string>
#include <vector>

#define SHOOT 's'
#define LEFT 'l'
#define RIGHT 'r'
#define COMMENT '#'

/**
 * @brief The Config class
 * Represents a game config, which controls what happens in the game
 */
class Config
{
    std::vector<std::string> data; // has all lines in config file

    std::string size; // size of spaceship
    int x_pos; // initial x-coordinate of spaceship

    std::string shape; // shape of alien
    int a_num; // alien nums
    int t_inter; // alien beam timer

    std::vector<char> actions; // list of actions (left, right, shoot)
    std::vector<int> times; // list of durations for each action
    int count; // keeps track of which action the spaceship is up to

public:
    Config(std::string filename);
    virtual ~Config() {}

    void setSize(std::string size) { this->size = size; }
    void setXPos(int x_pos) { this->x_pos = x_pos; }

    void setShape(std::string shape) { this->shape = shape; }
    void setANum(int an) { this->a_num = an; }
    void setTInter(int t) { this->t_inter = t; }

    const std::vector<std::string> &getData() const { return data; }

    std::string getSize() const { return size; }
    int getXPos() const { return x_pos; }

    std::string getShape() const { return shape; }
    int getANum() const { return a_num; }
    int getTInter() const { return t_inter; }

    // appends an action and duration to the "actions" and "times" vectors
    void pushAction(char action, int time);
    // returns the next action for the spaceship to execute
    char nextAction();
};

#endif // CONFIG_H
