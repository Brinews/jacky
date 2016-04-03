#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include "planet.h"

using namespace std;

class Sphere
{
public:
    Sphere();

    void addPlanets(string filename);
    void setView(float x, float y, float z);
    void doSimulation();

    vector<Planet *> getPlanets() {
        return stars;
    }

private:
    vector<Planet*> stars;
};

#endif // SPHERE_H
