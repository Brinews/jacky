
#include "ConvexPolygon.h"

ConvexPolygon::ConvexPolygon(const vector<glm::vec3> & vec, const color & material)
	: Plane(vec[0],normalize(cross(vec[1]-vec[0],vec[2]-vec[0])),material)
{
	this->vec = vec;
}


ConvexPolygon::~ConvexPolygon(void)
{
}
HitRecord ConvexPolygon::findClosestIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection)
{
	HitRecord hit = Plane::findClosestIntersection(rayOrigin, rayDirection);
	if (hit.t == FLT_MAX){
		return hit;
	}
	else {
		for (int i = 0; i < vec.size(); i++){
			if(0>glm::dot(glm::cross(vec[(i+1)%vec.size()]-vec[i],(hit.interceptPoint-vec[i])),hit.surfaceNormal)){
				hit.t = FLT_MAX;
				return hit;
			}
		}
	}
	return hit;

} // end checkIntercepts

