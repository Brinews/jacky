#pragma once

#include "RayTracer.h"

#include "Sphere.h"
#include "Plane.h"

/**
* Acts as the display function for the window. 
*/
static void RenderSceneCB();

// Reset viewport limits for full window rendering each time the window is resized.
static void ResizeCB(int width, int height);

// Responds to 'f' and escape keys. 'f' key allows 
// toggling full screen viewing. Escape key ends the
// program. Allows lights to be individually turned on and off.
static void KeyboardCB(unsigned char key, int x, int y);

// Responds to presses of the arrow keys
static void SpecialKeysCB(int key, int x, int y);

// Register as the "idle" function to have the screen continously
// repainted.
static void animate();

color getRandomColor();


