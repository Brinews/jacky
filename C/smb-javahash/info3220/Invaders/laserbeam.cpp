#include "laserbeam.h"

LaserBeam::LaserBeam(int x, int y) :
    x(x),
    y(y)
{
    s = 15;
    pic.load(":/images/fireball.png");
}

bool LaserBeam::move()
{
    if (y < 0) {
        // beam outside window, should be destroyed
        return false;
    }
    y -= s;
    return true;
}
