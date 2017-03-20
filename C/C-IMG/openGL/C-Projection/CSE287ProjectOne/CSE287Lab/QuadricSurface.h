#pragma once
#include "Surface.h"

/*

Super class to support intersection testing with quadric surfaces. These shapes can be
described by the general quadric surface equation 

Ax2 + By2 + Cz2 + Dxy+ Exz + Fyz + Gx + Hy + Iz + J = 0

*/
class QuadricSurface : 	public Surface
{
public:

	/**
	* Constructor for the sphere.
	* @param - position: specifies an xyz position of the center of the surface
	* @param - mat: color of the plane.
	*/
	QuadricSurface(const glm::vec3 & position, const color & mat);
	~QuadricSurface();

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
	* xyz location of the center of the surface
	*/
	glm::vec3 center;

	protected:

	/**
	* Coeficients is the  quadric surface equation
	* Ax2 + By2 + Cz2 + Dxy+ Exz + Fyz + Gx + Hy + Iz + J = 0
	*/
	float A, B, C, D, E, F, G, H, I, J;

};

