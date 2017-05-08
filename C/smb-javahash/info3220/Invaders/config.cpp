#include "config.h"
#include "defaults.h"
#include "iofile.h"

Config::Config(std::string filename)
{
    IOFile file(filename);
    data = file.getLines();

    count = 0;
}

void Config::pushAction(char action, int time)
{
    actions.push_back(action);
    times.push_back(time);
}

char Config::nextAction()
{
    if (count >= actions.size()) {
        // no more actions to execute
        return '\0';
    }

    // execute current action
    char action = actions.at(count);
    times.at(count) -= TICK;

    if (times.at(count) <= 0) {
        // duration of action has completely elapsed
        // prepare to execute next action
        count++;
    }

    return action;
}
