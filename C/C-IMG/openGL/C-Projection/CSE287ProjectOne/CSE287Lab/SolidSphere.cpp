#include "SolidSphere.h"

SolidSphere::SolidSphere(const glm::vec3 & position, const color & material, const float x, const float y, const float z)
	:QuadricSurface(position,material){
	A = x;
	B = y;
	C = z;
}
SolidSphere::~SolidSphere(void)
{
}

HitRecord SolidSphere::findClosestIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection){
	HitRecord hit = QuadricSurface::findClosestIntersection(rayOrigin, rayDirection);
	return hit;
}
