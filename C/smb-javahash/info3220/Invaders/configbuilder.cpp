#include "configbuilder.h"

#include "defaults.h"

#include <sstream>

ConfigBuilder::ConfigBuilder(std::string filename)
{
    conf = new Config(filename);
}

// this method does some validation (not all) of the file
void ConfigBuilder::setShip()
{
    const std::vector<std::string> &data = conf->getData();

    std::vector<std::string>::const_iterator itr = data.begin();
    // search file data for [Spaceship]
    while (itr != data.end() && *itr != "[Spaceship]") {
        itr++;
    }

    if (itr == data.end()) {
        // [Spaceship] does not exist in file data
        throw 1;
    }

    std::vector<std::string> tokens;

    // read all specified spaceship properties
    while (*itr != "[]") {
        const std::string &line = *itr;

        // ignore empty lines or comments (#)
        if (line == "" || line.at(0) == COMMENT) {
            ;
        } else {
            split(line, '=', tokens);

            std::string tag = tokens.at(0);
            if (tag == "Size") {
                // parse spaceship size
                std::string s = tokens.at(1);
                if (!(s == "tiny" ||
                      s == "normal" ||
                      s == "large" ||
                      s == "giant"))
                {
                    // specified size invalid, use default size
                    s = "normal";
                }
                conf->setSize(s);
            } else if (tag == "XPos") {
                // parse spaceship x-coordinate
                int x = std::stoi(tokens.at(1));
                if (x < 0 || x > W_WIDTH) {
                    // specific xPos invalid, use default xPos
                    x = 0;
                }
                conf->setXPos(x);
            }
        }

        itr++;
    }
}

// this method does some validation (not all) of the file
void ConfigBuilder::setActions()
{
    const std::vector<std::string> &data = conf->getData();

    std::vector<std::string>::const_iterator itr = data.begin();
    // search file data for [Actions]
    while (itr != data.end() && *itr != "[Actions]") {
        itr++;
    }

    if (itr == data.end()) {
        // [Actions] does not exist in file data
        throw 1;
    }

    std::vector<std::string> tokens;

    // read all specified spaceship actions
    while (*itr != "[]") {
        const std::string &line = *itr;

        // ignore empty lines or comments (#)
        if (line == "" || line.at(0) == COMMENT) {
            ;
        } else {
            split(line, ',', tokens);

            char move = tokens.at(0).at(0);
            int time = DURATION;
            if (tokens.size() > 1) {
                // use specified move duration
                time = std::stoi(tokens.at(1));
            }

            conf->pushAction(move, time);
        }

        itr++;
    }
}

// adapted from: https://stackoverflow.com/questions/236129/split-a-string-in-c#236803
void ConfigBuilder::split(const std::string &text, char delim, std::vector<std::string> &tokens) const
{
    std::stringstream ss;
    ss.str(text);
    std::string token;

    tokens.clear();
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
}

// this method does some validation (not all) of the file
void ConfigBuilder::setAlien()
{
    const std::vector<std::string> &data = conf->getData();

    std::vector<std::string>::const_iterator itr = data.begin();
    // search file data for [Spaceship]
    while (itr != data.end() && *itr != "[Alien]") {
        itr++;
    }

    if (itr == data.end()) {
        // [Spaceship] does not exist in file data
        throw 1;
    }

    std::vector<std::string> tokens;

    // read all specified spaceship properties
    while (*itr != "[]") {
        const std::string &line = *itr;

        // ignore empty lines or comments (#)
        if (line == "" || line.at(0) == COMMENT) {
            ;
        } else {
            split(line, '=', tokens);

            std::string tag = tokens.at(0);
            if (tag == "Shape") {
                // parse spaceship size
                std::string s = tokens.at(1);
                if (!(s == "shape1" ||
                      s == "shape2" ||
                      s == "shape3"))
                {
                    // specified size invalid, use default size
                    s = "shape1";
                }
                conf->setShape(s);
            } else if (tag == "ANum") {
                int n = std::stoi(tokens.at(1));
                if (n < 0 || n > MAX_ALIEN) {
                    n = 1;
                }
                conf->setANum(n);
            } else if (tag == "TBeam") {
                int t = std::stoi(tokens.at(1));
                conf->setTInter(t);
            }

        }

        itr++;
    }
}

