#include "BasicIncludesAndDefines.h"
#include "QuadricSurface.h"

class SolidSphere : public QuadricSurface{
public:

	SolidSphere::SolidSphere(const glm::vec3 & position, const color & material, const float x, const float y, const float z);
	virtual HitRecord findClosestIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection);
	glm::vec3 postiion;
	~SolidSphere(void);
};
