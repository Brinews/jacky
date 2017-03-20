#include "RayTracer.h"


RayTracer::RayTracer(FrameBuffer & cBuffer, color defaultColor )
:colorBuffer(cBuffer), defaultColor(defaultColor), recursionDepth(2)
{
	
}


RayTracer::~RayTracer(void)
{
}

void RayTracer::setCameraFrame(const glm::vec3 & viewPosition, const glm::vec3 & viewingDirection, glm::vec3 up)
{
	// Calculate and set the the w, u, and vdata members and 
	// set the eye data member to the viewing position.
	// TODO

	w = -1.0f * glm::normalize(viewingDirection);
    u = glm::normalize(glm::cross(up, w));
    v = glm::normalize(glm::cross(w, u));


} // end setCameraFrame


void RayTracer::calculatePerspectiveViewingParameters(const float verticalFieldOfViewDegrees)
{
	// Set the set the topLimit, bottomLimit, rightLimit, 
	// leftLimit, distToPlane, nx, and ny data members
	// TODO

    nx = (float)colorBuffer.getWindowWidth();
	ny = (float)colorBuffer.getWindowHeight();
 
    topLimit = 1;
    rightLimit = topLimit * (nx / ny);
    bottomLimit = (-1) * topLimit;
    leftLimit = (-1) * rightLimit;
 
    distToPlane = 1 / (tan(glm::radians(verticalFieldOfViewDegrees) / 2.0f));
	
} // end calculatePerspectiveViewingParameters


void RayTracer::calculateOrthographicViewingParameters(const float viewPlaneHeight)
{
	topLimit = fabs(viewPlaneHeight) / 2.0f;

	rightLimit = topLimit * ((float)colorBuffer.getWindowWidth()/colorBuffer.getWindowHeight()); // Set r based on aspect ratio and height of plane

	// Make view plane symetrical about the viewing direction
	leftLimit = -rightLimit; 
	bottomLimit = -topLimit;

	// Calculate the distance between pixels in the horizontal and vertical directions
	nx = (float)colorBuffer.getWindowWidth();
	ny = (float)colorBuffer.getWindowHeight();

	distToPlane = 0.0; // Rays start on the view plane
	
} // end calculateOrthographicViewingParameters


glm::vec2 RayTracer::getProjectionPlaneCoordinates(const int x, const int y)
{
	// Page 75
	// Calculate and return the u and v data members as the x an y coordinates
	// of the 
	glm::vec2 uvScalarValues;

    uvScalarValues.x = leftLimit + (((rightLimit - leftLimit) * ((x + 0.5))) / nx);
    uvScalarValues.y = bottomLimit + (((topLimit - bottomLimit) * ((y + 0.5))) / ny);
 
	// TODO
	return uvScalarValues;
}


void RayTracer::setOrthoRayOriginAndDirection(const int x, const int y)
{
	glm::vec2 uv = getProjectionPlaneCoordinates(x, y);

	// Page 75
	rayDirection = glm::normalize(-w);
	rayOrigin = eye + uv.x * u + uv.y * v;

} // end setOrthoOriginAndDirection


void RayTracer::setPerspectiveRayOriginAndDirection(const int x, const int y)
{

	// TODO
	rayOrigin = eye;
    glm::vec2 vec = getProjectionPlaneCoordinates(x, y);
    rayDirection = glm::normalize(-distToPlane * w + vec.x * u + vec.y * v);

} // end setPerspectiveOriginAndDirection





void RayTracer::raytraceScene(const std::vector<std::shared_ptr<Surface>> & surfaces, const std::vector<std::shared_ptr<LightSource>> & lights)
{
	this->surfacesInScene = surfaces;
	this->lightsInScene = lights;

	// Iterate through each and every pixel in the rendering window
	// TODO

	for (int i = 0; i < colorBuffer.getWindowHeight(); i++) {
		for (int j = 0; j < colorBuffer.getWindowWidth(); j++) {
			setPerspectiveRayOriginAndDirection(j, i);
            color c = traceIndividualRay(rayOrigin, rayDirection, 2);
			colorBuffer.setPixel(j, i, c);
		}
	}

} // end raytraceScene



color RayTracer::traceIndividualRay(const glm::vec3 &e, const glm::vec3 &d, int recursionLevel)
{
	// Find surface intersection that is closest to 'e' in the direction 'd.'
	// TODO

	if (recursionLevel <= 0) {
		return defaultColor;
	}

	HitRecord hr;
	hr.t = FLT_MAX;
	hr.material = defaultColor;

	color localLight;
    color c = defaultColor;

    float dist = FLT_MAX;
    for (int s = 0; s < (int)surfacesInScene.size(); s++) {
        HitRecord result = surfacesInScene[s]->findClosestIntersection(e, d);
        if (result.t != FLT_MAX) {
            if (dist > result.t) {
                dist = result.t;
                c = result.material;
            }
        }

		if (hr.t > result.t) {
			hr = result;
		}
    }

	for (int j = 0; j < (int)lightsInScene.size(); j++) {
		localLight += lightsInScene[j]->illuminate(-d, hr, surfacesInScene);
	}

	if (hr.t == FLT_MAX) {
		return localLight;
	}

	glm::vec3 reflection = d - 2 * glm::dot(d, hr.surfaceNormal) * hr.surfaceNormal;

	return localLight+traceIndividualRay(hr.interceptPoint + (EPSILON * hr.surfaceNormal),
		-reflection, recursionLevel - 1);
 
	//return c;
	

} // end traceRay







