#include "Surface.h"


Surface::Surface(const color & mat)
	:material(mat)
{
}


Surface::~Surface(void)
{
}

HitRecord Surface::findClosestIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection)
{
	HitRecord hitRecord;
	hitRecord.t = FLT_MAX;

	return hitRecord;
}