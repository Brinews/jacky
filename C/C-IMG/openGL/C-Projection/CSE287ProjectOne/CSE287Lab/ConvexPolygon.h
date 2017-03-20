#pragma once

#include "BasicIncludesAndDefines.h"
#include "Plane.h"
#include <vector>

using namespace std;

class ConvexPolygon : public Plane
{
public:

	ConvexPolygon(const vector<glm::vec3> & vec, const color & material);
	~ConvexPolygon(void);

	virtual HitRecord findClosestIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection);

	vector<glm::vec3> vec;

};
