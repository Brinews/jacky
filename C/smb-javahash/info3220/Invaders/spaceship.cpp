#include "defaults.h"
#include "spaceship.h"

Spaceship::Spaceship(std::string size, int x) :
    Vehicle(x)
{
    // load appropriate image
    if (size == "tiny") {
        pic.load(":/images/ship_tiny.png");
    } else if (size == "large") {
        pic.load(":/images/ship_large.png");
    } else if (size == "giant") {
        pic.load(":/images/ship_giant.png");
    } else {
        pic.load(":/images/ship_normal.png");
    }

    std::cout << pic.height() << endl;

    // set appropriate height of spaceship
    y = W_HEIGHT - pic.height();
}
