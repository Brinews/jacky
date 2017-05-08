#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "vehicle.h"

#include <QDialog>
#include <QPainter>
#include <QPixmap>

/**
 * @brief The Spaceship class
 * Represents the spaceship, which is a vehicle
 */
class Spaceship : public Vehicle
{
public:
    Spaceship(std::string size, int x);
    virtual ~Spaceship() {}
};

#endif // SPACESHIP_H
