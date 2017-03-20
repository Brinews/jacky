#include "Sphere.h"


Sphere::Sphere(const glm::vec3 & position, float radius, const color & material)
	: Surface(material), center(position), radius(radius)
{
}


Sphere::~Sphere(void)
{
}

/*
* Checks a ray for intersection with the surface. Finds the closest point of intersection
* if one exits. Returns a HitRecord with the t parmeter set to FLT_MAX if there is no
* intersection.
*/
HitRecord Sphere::findClosestIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection)
{
	HitRecord hitRecord;

	// Page 77
	float discriminant = pow(glm::dot(rayDirection, rayOrigin - center), 2) - dot(rayDirection, rayDirection)*(glm::dot(rayOrigin - center, rayOrigin - center) - radius * radius);

	if( discriminant >= 0 ) {

		float t = FLT_MAX;

		if( discriminant > 0 ) {

			// Two intercepts. Find and return the closest one.
			float t1 = (glm::dot(-rayDirection, rayOrigin - center) - sqrt(discriminant)) / dot(rayDirection, rayDirection);
			float t2 = (glm::dot(-rayDirection, rayOrigin - center) + sqrt(discriminant)) / dot(rayDirection, rayDirection);
	
			if (t1 < 0) {
				t1 = FLT_MAX;
			}
			if (t2 < 0) {
				t2 = FLT_MAX;
			}

			if (t1 < t2) {

				t = t1;
			}
			else {

				t = t2;
			}
		}
		else {
			// One Intercept. Find and return the t for the single point of intersection.
			t = glm::dot(-rayDirection, rayOrigin - center) / dot(rayDirection, rayDirection);
			if (t < 0) {
				t = FLT_MAX;
			}
		}

		// Set hit record information about the intersetion.
		hitRecord.t = t;
		hitRecord.interceptPoint = rayOrigin + t * rayDirection;
		
		glm::vec3 n = glm::normalize(hitRecord.interceptPoint - center);
		
		// Check for back face intersection
		if (glm::dot(n, rayDirection) > 0) {

			n = -n; // reverse the normal
		}

		hitRecord.surfaceNormal = n;
		hitRecord.material = material;
	}
	else {
		// Set parameter, t, in the hit record to indicate "no intersection."
		hitRecord.t = FLT_MAX;
	}

	return hitRecord;

} // end checkIntercept