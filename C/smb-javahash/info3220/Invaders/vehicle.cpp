#include "defaults.h"
#include "vehicle.h"

Vehicle::Vehicle(int x, int y) :
    x(x),
    y(y)
{
    s = 3;
}

Vehicle::~Vehicle()
{
    // delete each beam
    std::list<LaserBeam*>::iterator itr = beams.begin();
    while (itr != beams.end()) {
        LaserBeam *b = *itr;
        delete b;
        itr++;
    }
}

void Vehicle::moveLeft() {
    // move to left only if there is space to move to
    if (x > 0) {
        x -= s;
    }
}

void Vehicle::moveRight() {
    // move to right only if there is space to move to
    if (x + pic.width() < W_WIDTH) {
        x += s;
    }
}

void Vehicle::shoot() {
    // shoot a beam from the center of the spaceship
    beams.push_back(new LaserBeam(x + pic.width() / 2, y));
}

void Vehicle::animateBeams() {
    // animate existing laser beams, destroy if out of game window
    std::list<LaserBeam*>::iterator itr = beams.begin();
    while (itr != beams.end()) {
        LaserBeam *b = *itr;

        bool exists = b->move();
        if (exists) {
            itr++;
        } else {
            delete b;
            itr = beams.erase(itr);
        }
    }
}

bool Vehicle::collision(Vehicle *v) {
    // animate existing laser beams
    std::list<LaserBeam*>::iterator itr = beams.begin();
    while (itr != beams.end()) {
        LaserBeam *b = *itr;

        if (b->getX() > v->getX() && b->getX() < v->getX()+v->getPixmap().width()) {
            if (b->getY() > v->getY() && b->getY() < v->getY()+v->getPixmap().height()) {
                return true;
            }
        }

        itr++;
    }

    return false;
}
