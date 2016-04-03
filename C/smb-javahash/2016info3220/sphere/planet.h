#ifndef PLANET_H
#define PLANET_H

#include <string>


using namespace std;

class Planet
{
public:
    Planet();

    Planet(string line);

    void setSize(float s);
    void setColor(float r, float g, float b);
    void setPosition(float x, float y, float z);
    void setRotation(float r);
    void setSelfRotation(float sr);

    float getColorR() { return Rcolor; }
    float getColorG() { return Gcolor; }
    float getColorB() { return Bcolor; }

    float getPosX() { return posx; }
    float getPosY() { return posy; }
    float getPosZ() { return posz; }

    float getSize() { return size; }

    void drawPlanet();

private:
    char name[16];
    float size;
    float Rcolor, Gcolor, Bcolor;
    float mass;
    float posx, posy, posz;
    float rotation, selfrotation;
};

#endif // PLANET_H
