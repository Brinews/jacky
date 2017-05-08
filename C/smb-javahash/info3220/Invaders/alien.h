#ifndef ALIEN_H
#define ALIEN_H

#include "vehicle.h"

#include <QDialog>
#include <QPainter>
#include <QPixmap>

/**
 * @brief The Alien class
 * Represents the alien, which is a vehicle
 */
class Alien : public Vehicle
{
private:
    int inter;
    int timer;
public:
    Alien(std::string shape, int inter);
    virtual ~Alien() {}
    void updateTimer(int tick) {
        timer -= tick;
    }

    virtual void shoot();
};

#endif 
