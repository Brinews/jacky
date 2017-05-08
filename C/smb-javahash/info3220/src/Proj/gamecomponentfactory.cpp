#include "gamecomponentfactory.h"
#include <QDebug>

using namespace std;

gameComponentFactory::gameComponentFactory()
{
}

gameComponentFactory::~gameComponentFactory()
{
}

gameComponentType gameComponentFactory::stringTogameComponentType(
        const string& type) const
{
    if(type == "ship") return ship;
    if(type == "spaceships") return spaceships;
    if(type == "space") return space;

    throw invalid_argument(type + " is not a valid gameComponent type");
}

gameComponent* gameComponentFactory::creategameComponent(
        const unordered_map<string, string>& block) const
{
    string name;
    try
    {
        name = block.at("name");
    }
    catch(...)
    {
        throw invalid_argument("A block was missing the name field");
    }

    string parentName = "";
    if(block.count("parent") > 0)
    {
        parentName = block.at("parent");
    }

    //this key is guaranteed to exist in the block (it's the heading)
    gameComponentType type = stringTogameComponentType(block.at("category"));
    switch(type)
    {
    case ship:
        return createLeaf(block, name, parentName, type);
    default:
        return createComposite(block, name, parentName, type);
    }
}

QColor gameComponentFactory::parseColor(const string& value) const
{
    QColor color(value.c_str());
    if(color.isValid())
    {
        return color;
    }
    qDebug() << "Invalid color, should be like #FF8800, but was:" << value.c_str();
    return Qt::white;
}

BodySize gameComponentFactory::parseSize(const string& value) const
{
    if (value == "small") return small;
    if (value == "middle") return middle;
    if (value == "big") return big;

    return small;
}

double gameComponentFactory::getDouble(
        const unordered_map<string, string>& block,
        const string& key,
        const string& name,
        bool required) const
{
    //fetch the value
    if(block.count(key) < 1)
    {
        if(required)
        {
            throw out_of_range("Error building " + name + ": missing required value: " + key);
        }
        //key was not in the block, but is not required, use 0.0 as a default
        return 0.0;
    }
    //try to convert the value to a double
    bool ok;
    double value = QString::fromStdString(block.at(key)).toDouble(&ok);
    if(!ok)
    {
        throw invalid_argument("Error building " + name + ": " + block.at(key) + " is not a double");
    }
    //success
    return value;
}

gameComponent* gameComponentFactory::createLeaf(
        const unordered_map<string, string>& block,
        const string& name,
        const string& parentName,
        gameComponentType type) const
{
    //fetch various doubles from the block. Throw errors if they are missing or invalid.
    double position_x = getDouble(block, "position_x", name, true);
    double position_y = getDouble(block, "position_y", name, true);
    double velocity_x = getDouble(block, "velocity_x", name, true);
    double velocity_y = getDouble(block, "velocity_y", name, true);

    //build the body:
    gameBody* component = new gameBody(type, name, parentName);
    component->setPosition(position_x, position_y);
    component->setVelocity(velocity_x, velocity_y);

    //use the colour argument, only if it exists
    if(block.count("color") > 0)
    {
        component->setColor(parseColor(block.at("color")));
    }

    if (block.count("size") > 0) {
        component->setSize(parseSize(block.at("size")));
    }

    return component;
}


gameComponent* gameComponentFactory::createComposite(
        const unordered_map<string, string>& block,
        const string& name,
        const string& parentName,
        gameComponentType type) const
{
    //fetch various doubles from the block. Throw errors only if they are invalid
    double position_x = getDouble(block, "position_x", name, false);
    double position_y = getDouble(block, "position_y", name, false);
    double velocity_x = getDouble(block, "velocity_x", name, false);
    double velocity_y = getDouble(block, "velocity_y", name, false);

    //build the body:
    gameComposite* component = new gameComposite(type, name, parentName);
    component->setPosition(position_x, position_y);
    component->setVelocity(velocity_x, velocity_y);

    return component;
}
