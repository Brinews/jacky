#include <iostream>
#include <cmath>
#include "easygl.h"
#include "Circle.h"
using namespace std;


// Constructor. First set up base class (Shape), then store the 
// center and radius
Circle::Circle (string _name, string _colour, float _xcen, float _ycen,
            float _radius) 
             : Shape (_name, _colour, _xcen, _ycen) {
    radius = _radius;
}


Circle::~Circle () {
   // No dynamic memory to delete
}


void Circle::print () const {
   Shape::print();
   cout << "circle radius: " << radius;
   cout << endl;
}


void Circle::scale (float scaleFac) {
    radius *= scaleFac;
}
   

float Circle::computeArea () const {
   // pi*r*r
   float area;
   area = PI * radius * radius;
   return area;
}


float Circle::computePerimeter () const {
   // 2*PI*r
   float perimeter = 0;
   
   perimeter = 2*PI*radius;
   
   return perimeter;
}


void Circle::draw (easygl* window) const {
   // Load up the data structure needed by easygl, and draw the circle
   // using the easygl::gl_fillarc call.
   window->gl_setcolor(getColour());
   window->gl_fillarc(getXcen(), getYcen(), radius, 0, 360);
}


bool Circle::pointInside (float x, float y) const {
   // Make a point that corresponds to where you clicked.
   t_point click;
   click.x = x - getXcen();
   click.y = y - getYcen();
   
   if (click.x*click.x + click.y*click.y < radius*radius) return true;
   
   return false;
}

