#pragma once

#include "Surface.h"

/**
* Sub-class of Surface that represents inplicit description of a sphere.
*/
class Sphere : 	public Surface
{
	public:

	/**
	* Constructor for the sphere.
	* @param - point: specifies an xyz position of the center of the sphere
	* @param - radius: radius of the sphere
	* @param - material: color of the plane.
	*/
	Sphere(const glm::vec3 & position = glm::vec3(0.0f, 0.0f, -5.0f),
			float radius = 1.0f, 
			const color & material = color(1.0f, 1.0f, 1.0f, 1.0f) );

	~Sphere(void);

	/**
	* Checks a ray for intersection with the surface. Finds the closest point of intersection
	* if one exits. Returns a HitRecord with the t parmeter set to FLT_MAX if there is no
	* intersection.
	* @param rayOrigin - Origin of the ray being check for intersetion
	* @param rayDirection - Unit vector represention the direction of the ray.
	* returns HitRecord containing intormation about the point of intersection.
	*/
	virtual HitRecord findClosestIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection);

	/**
	* Radius of the sphere
	*/
	float radius;

	/**
	* xyz location of the center of the sphere
	*/
	glm::vec3 center;
};

