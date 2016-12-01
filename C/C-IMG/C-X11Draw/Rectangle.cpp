#include <iostream>
#include <cmath>
#include "easygl.h"
#include "Rectangle.h"
using namespace std;


// Constructor. First set up base class (Shape), then store the 
// vertices, which are 2 (x,y) points giving offsets from the Shape center 
// to each rectangle vertex.
Rectangle::Rectangle (string _name, string _colour, float _xcen, float _ycen,
            float _width, float _height) 
              : Shape (_name, _colour, _xcen, _ycen) {
   
   width = _width;
   height = _height;
}


Rectangle::~Rectangle () {
   // No dynamic memory to delete
}


void Rectangle::print () const {
   Shape::print();
   cout << "rectangle width: ";
   cout << width << " height: " << height;

   cout << endl;
}


void Rectangle::scale (float scaleFac) {
   width *= scaleFac;
   height *= scaleFac;
}
   

float Rectangle::computeArea () const {
   // width * height
   float area;

   area = width*height;
   return area;
}


float Rectangle::computePerimeter () const {
   float perimeter = 0;
   
   perimeter = 2 * (width + height);
   return perimeter;
}

void Rectangle::draw (easygl* window) const {
   // Load up the data structure needed by easygl, and draw the rectangle
   // using the easygl::gl_fillrect call.
   t_point coords[2];  // This data type is in easygl_constants.h

   coords[0].x = getXcen() - width/2;
   coords[0].y = getYcen() - height/2;

   coords[1].x = getXcen() + width/2;
   coords[1].y = getYcen() + height/2;

   window->gl_setcolor(getColour());
   window->gl_fillrect(coords[0].x, coords[0].y, coords[1].x, coords[1].y);
}


bool Rectangle::pointInside (float x, float y) const {
   t_point click;
   click.x = x - getXcen();
   click.y = y - getYcen();
   
   if (abs(click.x) < width/2 && abs(click.y) < height/2)
       return true;
   
   return false;
}


// Private helper functions below.  Working out triangle area etc. 
// requires some vector math, so these helper functions are useful.

double Rectangle::crossMultiply (t_point start, t_point end, t_point dot) const {
   return ((start.x-dot.x)*(end.y-dot.y)-(end.x-dot.x)*(start.y-dot.y));
}

