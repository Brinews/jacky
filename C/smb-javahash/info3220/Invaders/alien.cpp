#include "defaults.h"
#include "alien.h"


Alien::Alien(std::string shape, int in) :
    Vehicle(50, 50)
{
    // load appropriate image
    if (shape == "shape1") {
        pic.load(":/images/shape1.png");
    } else if (shape == "shape2") {
        pic.load(":/images/shape2.png");
    } else if (shape == "shape3") {
        pic.load(":/images/shape3.png");
    }

    // set appropriate height of alien
    y = pic.height();
    inter = in;
    timer = in;
}

void Alien::shoot() {
    if (timer <= 0) {
        LaserBeam *lb = new LaserBeam(x + pic.width() / 2, y+pic.height());
        lb->setS(-15);
        beams.push_back(lb);
        timer = inter;
    }
}
