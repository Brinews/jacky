#include "planet.h"

#include <iostream>
#include <sstream>

#include <QGLWidget>
#include <GL/glut.h>

using namespace std;

Planet::Planet()
{
}

Planet::Planet(string line)
{
    istringstream fsin(line);

    fsin >> name;
    fsin >> size;
    fsin >> Rcolor >> Gcolor >> Bcolor;
    fsin >> mass;
    fsin >> posx >> posy >> posz;
    fsin >> rotation >> selfrotation;
}

void Planet::setSize(float s)
{
    size = s;
}

void Planet::setColor(float r, float g, float b)
{
    Rcolor = r;
    Gcolor = g;
    Bcolor = b;
}

void Planet::setPosition(float x, float y, float z)
{
    posx = x;
    posy = y;
    posz = z;
}

void Planet::setRotation(float r)
{
    rotation = r;
}

void Planet::setSelfRotation(float sr)
{
    selfrotation = sr;
}

void Planet::drawPlanet()
{
    /* todo */

}
