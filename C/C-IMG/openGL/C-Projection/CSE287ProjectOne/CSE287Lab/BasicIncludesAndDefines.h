#pragma once

#include <iostream> // Stream input and output operations
#include <vector> // Sequence containers for arrays that can change in size
#include <memory> // General utilities to manage dynamic memory

// Glut takes care of all the system-specific chores required for creating windows, 
// initializing OpenGL contexts, and handling input events
#include <GL/freeglut.h>

// GLM math library includes (See http://glm.g-truc.net/0.9.7/glm-0.9.7.pdf)
#define GLM_SWIZZLE  // Enable GLM "swizzle" operators

// Basic GLM functionality
#include <glm/glm.hpp> 

// Stable glm extensions
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/constants.hpp>

// Experimental GLM extensions.
#include <glm/gtx/rotate_vector.hpp>
//#include <glm/gtx/euler_angles.hpp>
//#include <glm/gtx/quaternion.hpp>

// #defines for text substitution in source code prior to compile

#define color glm::vec4

// Attenuation factors
#define CONSTANT_ATTEN 1.0f
#define LINEAR_ATTEN 0.01f
#define QUADRATIC_ATTEN 0.001f

#define WINDOW_WIDTH 512 // Default window width in pixels
#define WINDOW_HEIGHT 316 // Default window height in pixels = width/1.618

// Small value used to create offset to avoid "surface acne"
#define EPSILON 1.0E-4f

#define M_PI glm::pi<float>()

// Function for generating random colors. Alpha value is always 
// set to 1.0
color getRandomColor();

// Simple functions for printing vectors and matrices to the console
void print(const glm::vec2 & v0);

void print(const glm::vec3 & v0);

void print(const glm::vec4 & v0);

void print(const glm::mat2 & m);

void print(const glm::mat3 & m);

void print(const glm::mat4 & m);

