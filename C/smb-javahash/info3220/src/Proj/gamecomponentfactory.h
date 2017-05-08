#ifndef game_FACTORY_H
#define game_FACTORY_H

#include "gamebody.h"
#include "gamecomponent.h"
#include "gamecomposite.h"
#include <QColor>
#include <string>
#include <unordered_map>


class gameComponentFactory
{
public:
    gameComponentFactory();
    virtual ~gameComponentFactory();

    //This is what's known as a "Parameterised factory method"
    //It decides which class to create based on the parameter passed in
    //It also handles the construction of the complex object, like a Builder
    virtual gameComponent* creategameComponent(
            const std::unordered_map<std::string, std::string>& block) const;

private:
    //Create a leaf type (planet, ship, blackhole)
    virtual gameComponent* createLeaf(
            const std::unordered_map<std::string, std::string>& block,
            const std::string& name,
            const std::string& parentName,
            gameComponentType type) const;

    //Create a composite type (space, galaxy, spaceships)
    virtual gameComponent* createComposite(
            const std::unordered_map<std::string, std::string>& block,
            const std::string& name,
            const std::string& parentName,
            gameComponentType type) const;

    //convert a #123456 colour to a QColor
    QColor parseColor(const std::string& value) const;
    BodySize parseSize(const std::string& value) const;

    //get a double from a block
    double getDouble(
            const std::unordered_map<std::string, std::string>& block,
            const std::string& key,
            const std::string& name,
            bool required = true) const;

    //convert a string to a gameComponentType
    gameComponentType stringTogameComponentType(const std::string& type) const;

};

#endif // FACTORY_H
