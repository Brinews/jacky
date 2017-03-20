#pragma once

#include "BasicIncludesAndDefines.h"

/**
* Simple struct to hold information about points of intersection.
*/
struct HitRecord {

	HitRecord(){ t = FLT_MAX; }

	glm::vec3 interceptPoint; // xyz location of intersection

	glm::vec3 surfaceNormal; // surface normal at the point of intersection

	color material; // Color of the surface

	float t; // Paremeter in parametric a ray at point of intersectopm

};