#pragma once

#include "FrameBuffer.h"
#include "Lights.h"
#include "HitRecord.h"
#include "Surface.h"

/**
* Class that supports simple ray tracing of a scene containing a number of object 
* (surfaces) and light sources.
*/
class RayTracer
{
	public:

	/**
	* Constructor.
	* @param color buffer to which the ray tracer will be render.
	* @param default color to which pixel will be set when there it no intersection
	* for a ray associated with a particular pixel.
	*/
	RayTracer(FrameBuffer & cBuffer, color defaultColor = color(0.0, 0.0, 0.0, 1.0));

	~RayTracer(void);

	/**
	* Ray traces a scene containing a number of surfaces and light sources. Sets every
	* pixel in the rendering window. Pixels that are not associated with a ray/surface
	* intersection are set to a default color.
	* @param surfaces - list of the surfaces in the scene
	* @param lights - list of the light sources in the scene
	*/
	void raytraceScene(const std::vector<std::shared_ptr<Surface>> & surfaces, const std::vector<std::shared_ptr<LightSource>> & lights);

	/**
	* Sets the w, u, and v orthonormal basis vectors associated with the coordinate
	* frame that is tied to the viewing position and the eye data member of the
	* class.
	* @param viewPosition - xyz position of the view point
	* @param viewingDirection - vector that points in the viewing direction
	* @param up - approximation of the up vector (cannot be parallel to viewing direction)
	*/
	void setCameraFrame(const glm::vec3 & viewPosition, const glm::vec3 & viewingDirection, glm::vec3 up);

	/**
	* Set the following members of the class: topLimit, bottomLimit, rightLimit,
	* leftLimit, distToPlane, nx, and ny based the distance form the middle of
	* the projection plane to the top of the projection plane and width of the
	* rendering window.
	* @param viewPlaneHeight - distance to the top of the projection plane
	*/
	void calculateOrthographicViewingParameters(const float viewPlaneHeight = 10.0f);

	/**
	* Set the following members of the class: topLimit, bottomLimit, rightLimit,
	* leftLimit, distToPlane, nx, and ny based of the specified vertical field of
	* view and height and width of the rendering window.
	* @param verticalFieldOfViewDegrees - vertical field of view in degrees
	*/
	void calculatePerspectiveViewingParameters(const float verticalFieldOfViewDegrees = 45.0f);

	/**
	* Set the color to which pixels are set when the associated ray does not
	* intersect any object in the scene.
	* @param default - color for pixels for which no ray-object intersections occur.
	*/
	void setDefaultColor(color default) { this->defaultColor = default; };

	int recursionDepth;

protected:

	/**
	* Once the closest point of intersection is found a color is returned based on
	* calculated interactions between the intersected surface and the light
	* sources in the scene.
	*
	* Can be called recursively to trace rays associated with reflection and
	* refraction.
	* @param e - origin of the ray being traced
	* @param d - unit length vector representing the direction of the ray
	* @returns color for the point of intersection
	*/
	color traceIndividualRay(const glm::vec3 &e, const glm::vec3 &d);
	color traceIndividualRay(const glm::vec3 &e, const glm::vec3 &d, int recursionLevel = 0);
	
	/**
	* Checks the ray for intersection against every Surface in the scene. It does
	* this by repeatedly passing the ray origin (e) and the ray direction (d) to
	* findClosestIntersection methods of Surface class and its sub-classes.
	* 
	* @param e - origin of the ray being traced
	* @param d - unit length vector representing the direction of the ray
	* @returns HitRecord of the intersection, if one exists, or a HitRecord with t of FLT_MAX otherwise
	*/
	HitRecord findIntersectingHit(const glm::vec3 &e, const glm::vec3 &d);

	/**
	* Sets the rayOrigin and rayDirection data members of the class based on row and
	* column of a specified pixel.These two data members together represent a
	* parametric description of a ray to be checked for intersection with objects
	* in the scene. The ray is caluclated for a orthographic projection.
	* @param x column of a pixel in the rendering window
	* @param y row of a pixel in the rendering window
	*/
	void setOrthoRayOriginAndDirection( const int x, const int y);

	/**
	* Sets the rayOrigin and rayDirection data members of the class based on row and 
	* column of a specified pixel.These two data members together represent a 
	* parametric description of a ray to be checked for intersection with objects
	* in the scene. The ray is caluclated for a perspective projection.
	* @param x column of a pixel in the rendering window
	* @param y row of a pixel in the rendering window
	*/void setPerspectiveRayOriginAndDirection( const int x, const int y);

	/**
	* Finds the projection plane coordinates, u and v, for the pixel identified
	* by the input arguments.
	* @param x column of a pixel in the rendering window
	* @param y row of a pixel in the rendering window
	* @returns two dimensional vector containing the projection plane coordinates
	*/
	glm::vec2 getProjectionPlaneCoordinates(const int x, const int y);

	// Alias for an object controls memory that stores a rgba color value f
	// or every pixel.
	FrameBuffer & colorBuffer;

	// Color to which a pixel is set if there is no intersection
	// for the traced pixel ray.
	color defaultColor;

	// View frame parameters
	glm::vec3 eye; // position of the viewpoint
	glm::vec3 u; // "right" relative to the viewing direction
	glm::vec3 v; // perpendicular up vector
	glm::vec3 w; // camera looks in the negative w direction

	// Projection plane parameters
	// Measured relative to u (right)
	float rightLimit;
	float leftLimit;
	// Measured reltive to v (up)
	float topLimit;
	float bottomLimit;

	// Rendering window parameters
	float nx; // Width in pixels
	float ny; // Height in pixel

	// Distance from the viewpoint to the projection plane
	float distToPlane;

	// Parametric ray being traced for a pixel
	glm::vec3 rayDirection; // Direction of the ray
	glm::vec3 rayOrigin; // Origin of the ray

	// List of the surfaces in the scene that is being ray traced
	std::vector<std::shared_ptr<Surface>> surfacesInScene;

	// List of the light sources in the scene that is being ray traced
	std::vector<std::shared_ptr<LightSource>> lightsInScene;

};


