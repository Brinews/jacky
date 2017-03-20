#pragma once
#include "BasicIncludesAndDefines.h"
/**
* Simple stuct to hold the material properties of a surface
*/
struct Material {
	/**
	* Constructor that initializes the material properties of the
	* surface. By default, the object will be white with no
	* emmisive color
	*/
	Material( color ambDifColor = color(1.0f, 1.0f, 1.0f, 1.0f),
		color specColor = color(1.0f, 1.0f, 1.0f, 1.0f),
		float shininess = 128.0f )
		: ambientColor( ambDifColor ), diffuseColor ( ambDifColor ),
		specularColor (specColor), emissiveColor(0.0f, 0.0f, 0.0f, 1.0f),
		shininess( shininess )
	{}
	// Ambient color of the surface
	color ambientColor;
	// Diffuse color of the surface
	color diffuseColor;
	// Specular color of the surface
	color specularColor;
	// Emissive color of the surface
	color emissiveColor;
	// Shininess exponent
	float shininess;
};