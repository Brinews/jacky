#include "sphere.h"

#include <fstream>
#include <string>

using namespace std;

Sphere::Sphere()
{
}

void Sphere::addPlanets(string filename)
{
    ifstream fin(filename);
    string line;

    while (getline(fin, line)) {
        if (line[0] == '#') continue;

        Planet *p = new Planet(line);

        stars.push_back(p);
    }
}

void Sphere::setView(float x, float y, float z)
{
    /* to do */
}

void Sphere::doSimulation()
{
    for (int i = 0; i < stars.size(); i++) {
        stars[i]->drawPlanet();
    }
}
