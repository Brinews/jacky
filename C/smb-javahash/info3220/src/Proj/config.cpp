#include "config.h"
#include "gamecomposite.h"
#include "gamecomponentfactory.h"
#include <QDebug>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <sstream>


using namespace std;

//Singleton variables
bool Config::s_initialised = false;
Config* Config::s_instance = nullptr;


//a type to make it easier to keep track of what the parser is doing
enum BlockType { none, settings, component };

Config::Config()
    : m_framesPerSecond(60)
    , m_physicsStepSize(40320)
    , m_overcalculatePhysics(1)
    , m_distanceScale(1.0e10)

{
}

Config::~Config()
{
}

Config* Config::getInstance()
{
    if(!s_initialised)
    {
        s_instance = new Config();
        s_initialised = true;
    }
    return s_instance;
}


string Config::stripComment(const string& line) const
{
    if(line.length() == 0)
    {
        return line;
    }
    int index = line.find_first_of('/');
    return line.substr(0, index);
}

bool Config::isBlockHeading(const string& line) const
{
    return (line.length() > 0)
            && (line[0] == '[')
            && (line[line.length()-1] == ']');
}

bool Config::isKeyValuePair(const string& key, const string& op, const string& value) const
{
    return (key.length() > 0) && (op == "=") && (value.length() > 0);
}

//acquire file, read strings, close file
void Config::readLines(const string& path, list<string>& output) const
{
    ifstream configfile(path.c_str());
    if(!configfile.is_open()) {
        qDebug() << "Unable to open file: " << path.c_str();
    }
    string line;
    while(getline(configfile, line))
    {
        output.push_back(line);
    }
    configfile.close();
}

void Config::read(const string& path)
{
    m_settings.clear();
    m_gameBlocks.clear();

    unordered_map<string, string> block;

    BlockType currentBlockType = none;

    list<string> lines;

    readLines(path, lines);
    int lineNumber = 0;

    list<string>::iterator line = lines.begin();

    for (; line != lines.end(); line++)
    {
        ++lineNumber;

        //try to read three strings (breaks on spaces!)
        stringstream ss(stripComment(*line));
        string key, op, value;
        ss >> key >> op >> value;

        //skip blank lines silently
        if(key.length() == 0)
        {
            continue;
        }

        //shipting a new heading
        if(isBlockHeading(key))
        {
            //save the finished block
            switch(currentBlockType)
            {
            case component:
                m_gameBlocks.emplace_back(block);
                block.clear();
                break;

            case settings:
            case none:
                break;
            }

            //shipt the new block
            string label = key.substr(1, key.length()-2);
            if(label == "settings")
            {
                currentBlockType = settings;
            }
            else
            {
                currentBlockType = component;
                block.clear();
                block.insert(make_pair("category", label));
                //block = {{"category", label }};
            }
        }
        //read a k-v pair within a heading
        else if(isKeyValuePair(key, op, value))
        {
            switch(currentBlockType)
            {
            case settings:
                m_settings.insert(make_pair(key, value));
                break;
            case component:
                block.insert(make_pair(key, value));
                break;
            case none:
            default:
                qDebug() << "ERROR on line" << lineNumber << ", file should have shipted with a [heading]";
            }
        }
        else
        {
            qDebug() << "ERROR on line" << lineNumber
                     << "Expected: 'key = value' or '[block]'"
                     << "actually:"
                     << (*line).c_str();
        }
    }

    //end of file, save the finished block
    switch(currentBlockType)
    {
    case component:
        m_gameBlocks.emplace_back(block);
        block.clear();
        break;

    case settings:
    case none:
        break;
    }

    //update the settings
    parseSettings();
}

void Config::parseSettings()
{
    if(m_settings.count("FRAMES_PER_SECOND") > 0)
    {
        stringstream ss(m_settings.at("FRAMES_PER_SECOND"));
        ss >> m_framesPerSecond;
        qDebug() << "FRAMES_PER_SECOND" << m_framesPerSecond;
    }

}

void Config::joinComponents(gameComponent* parent, gameComponent* child)
{
    switch(parent->getType())
    {
    //allow anything to join to the game
    case game:
        dynamic_cast<gameComposite*>(parent)->add(child);
        return;

    case space:
        switch(child->getType())
        {
        case spaceships:
            dynamic_cast<gameComposite*>(parent)->add(child);
            return;
        default:
            break;
        }
        break;

    //only allow ships to join to a spaceships
    case spaceships:
        switch(child->getType())
        {
        case ship:
            dynamic_cast<gameComposite*>(parent)->add(child);
            return;
        default:
            break;
        }
        break;
    default:
        break;
    }

    //anything else is invalid
    qDebug() << "ERROR: cannot add ("
             << child->getName().c_str()
             << ") to ("
             << parent->getName().c_str()
             << "). They are the wrong types. Skipping object.";
    return;

}


gameComponent* Config::parsegameBlocks()
{
    gameComponentFactory factory;
    gameComponent* game = new gameComposite(
                gameComponentType::game, "The game");

    m_gameComponents.clear();

    //create the components
    std::list<std::unordered_map<std::string, std::string> >::iterator itr;
    for (itr = m_gameBlocks.begin(); itr != m_gameBlocks.end(); itr++)
    {
        std::unordered_map<std::string, std::string> block = *itr;
        try
        {
            gameComponent* component = factory.creategameComponent(block);
            if(m_gameComponents.count(component->getName()) > 0)
            {
                qDebug() << "ERROR: name already in use (skipping):" << component->getName().c_str();
            }
            else
            {
                m_gameComponents.insert(make_pair(component->getName(), component));
            }
        }
        catch(invalid_argument e)
        {
            qDebug() << "ERROR: skipped an object:" << e.what();
        }
        catch(out_of_range e)
        {
            qDebug() << "ERROR: skipped an object:" << e.what();
        }
        catch(exception e)
        {
            qDebug() << "ERROR: skipped an object:" << e.what();
        }
    }

    //link the components to their parents (or to the game)
    std::unordered_map<std::string, gameComponent*>::iterator itr1;
    for (itr1 = m_gameComponents.begin(); itr1 != m_gameComponents.end(); itr1++)
    {
        std::pair<std::string, gameComponent*> pair = *itr1;

        gameComponent* child = pair.second;
        gameComponent* parent = game;
        if(child->getParentName() != "")
        {
            parent = m_gameComponents.at(child->getParentName());
        }
        joinComponents(parent, child);
    }

    return game;
}


