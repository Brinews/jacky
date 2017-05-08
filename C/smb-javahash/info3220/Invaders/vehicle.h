#ifndef VEHICLE_H
#define VEHICLE_H

#include "laserbeam.h"

#include <QDialog>
#include <QPainter>
#include <QPixmap>

#include <list>

/**
 * @brief The Vehicle class
 * Represents all vehicles (spaceships, aliens, ...)
 * Vehicles can move left, move right, and shoot
 */
class Vehicle
{
protected:
    QPixmap pic; // image of vehicle
    int x, y; // x, y coordinates of vehicle
    int s; // stride taken by vehicle, should it choose to move
    std::list<LaserBeam*> beams; // has all laser beams shot from vehicle

public:
    Vehicle(int x = 0, int y = 0);
    virtual ~Vehicle();

    virtual void moveLeft();
    virtual void moveRight();
    virtual void shoot();
    // animates (moves) all beams shot by vehicle
    virtual void animateBeams();

    virtual bool collision(Vehicle *v);

    const QPixmap &getPixmap() const { return pic; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getS() const { return s; }
    const std::list<LaserBeam*> &getBeams() const { return beams; }
};

#endif // VEHICLE_H
