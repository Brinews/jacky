#pragma once

#include "BasicIncludesAndDefines.h"
#include "HitRecord.h"
#include "Surface.h"

HitRecord findIntersection(const glm::vec3 &e, const glm::vec3 &d, const std::vector<std::shared_ptr<Surface>> & surfaces);


/**
* Base struct for all types of lights. Supports only specification of the 
* color and intensity of light sources. Parent of sub-structs that simulate
* positional, directional, and spot lights.
*
* Instances of this struct an be used to simulate only ambient light. Ambient
* light is described as bounced light that has been scattered so much that it
* is impossible to tell the direction to its source. If using a LightSource 
* instantiation to simulate ambient light, the overall intensity of the light
* should be low in order to avoid washing out shadows as well as diffuse and 
* specular effects that can be achieved when using children of this struct.
*/
struct LightSource
{
	LightSource(const color & lightColor) 
	: lightColor(lightColor)
	{}

	virtual color illuminate(const glm::vec3 & eyeVector, HitRecord & closestHit, const std::vector<std::shared_ptr<Surface>> & surfaces)
	{
		color ret_color;
		ret_color.r = closestHit.material.r *lightColor.r;
		ret_color.g = closestHit.material.g *lightColor.g;
		ret_color.b = closestHit.material.b *lightColor.b;
		ret_color.a = closestHit.material.a *lightColor.a;
		return ret_color;
		//return color(0.0f);
	}

	/* 
	* Color and intensity of the light.
	*/
	color lightColor; 
};

/**
* Struct for simulating light sources that have an explicit position and 
* shine equally in all directions. Instantiations of the struct will have
* a position property and a color and intensity of the light given off
* by the light.
*/
struct PositionalLight : public LightSource
{
	PositionalLight(glm::vec3 position, const color & lightColor)
	: LightSource(lightColor), lightPosition(position)
	{}

	virtual color illuminate(const glm::vec3 & eyeVector, HitRecord & closestHit, const std::vector<std::shared_ptr<Surface>> & surfaces)
	{
		int shiney = 255;
		color white = color(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec3 n = closestHit.surfaceNormal;
		glm::vec3 v = glm::normalize(eyeVector - closestHit.interceptPoint);
		glm::vec3 l = glm::normalize(lightPosition - closestHit.interceptPoint);
		glm::vec3 h = glm::normalize(v + l);

		HitRecord hr_temp;
		hr_temp.t = FLT_MAX;
		glm::vec3 point = closestHit.interceptPoint + (EPSILON * closestHit.surfaceNormal);
		for (int i = 0; i < surfaces.size(); i++){
			HitRecord hr = surfaces[i]->findClosestIntersection(point, -l);
			if (hr.t < hr_temp.t)
			{
				hr_temp = hr;
			}
		}

		float toLight = distance(point, lightPosition);
		float toCept = distance(point, hr_temp.interceptPoint);
		if (hr_temp.t < FLT_MAX  && hr_temp.t < 0 && toLight > toCept) {
			return color(0.0f, 0.0f, 0.0f, 1.0f);
		}
		
		color diffuse = glm::max(0.0f, dot(n, l))*lightColor*closestHit.material;
		color specular = pow(glm::max(0.0f, dot(n, h)), shiney)*white*white;

		return diffuse + specular;

		//return color(0.0f);
	}

	float distance(glm::vec3 a, glm::vec3 b)
	{
		return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
	}

	/**
	* x, y, z position of the light source. 
	*/
	glm::vec3 lightPosition; 
};

/**
* Struct for simulating light sources that do not have an explicit position.
* Such light sources have only a direction against which they are shinning.
* Instantiations of the struct will have this direction properties along with
* a color and intensity of the light given off by the light source.
*/
struct DirectionalLight : public LightSource
{
	DirectionalLight(glm::vec3 direction, const color & lightColor)
	: LightSource(lightColor), lightDirection(glm::normalize(direction))
	{}

	virtual color illuminate(const glm::vec3 & eyeVector, HitRecord & closestHit, const std::vector<std::shared_ptr<Surface>> & surfaces)
	{
		int shiney = 255;
		color white = color(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec3 n = closestHit.surfaceNormal;
		glm::vec3 v = glm::normalize(eyeVector - closestHit.interceptPoint);
		glm::vec3 l = glm::normalize(lightDirection - closestHit.interceptPoint);
		glm::vec3 h = glm::normalize(v + l);

		
		for (int i = 0; i < surfaces.size(); i++){
			glm::vec3 point = closestHit.interceptPoint + (EPSILON * closestHit.surfaceNormal);
			HitRecord hr = surfaces[i]->findClosestIntersection(point, l);
			if (hr.t < FLT_MAX)
			{
				if (hr.t < 0) {
					return color(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}
		}
		
		color diffuse = glm::max(0.0f, dot(n, l))*lightColor*closestHit.material;
		color specular = pow(glm::max(0.0f, dot(n, h)), shiney)*white*white;
		return diffuse + specular;
		//return color(0.0f);
	}

	/**
	* Unit vector that points in the direction that is opposite 
	* the direction in which the light is shining.
	*/
	glm::vec3 lightDirection; 
};


