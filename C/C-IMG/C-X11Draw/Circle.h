/* 
 * File:   Circle.h
 * Author: 
 *
 * Created on November 30, 2016
 */

#ifndef CIRCLE_H
#define	CIRCLE_H

#include "Shape.h"
#include "easygl.h"

class Circle : public Shape {
private:
   float radius;
   
public:
   Circle (string _name, string _colour, float _xcen, float _ycen,
            float _radius);
   
   // virtual keyword is optional in all the function declarations below, since
   // the base class Shape already declared them virtual. But it is good 
   // practice to repeat it again here, for clarity.
   virtual ~Circle();
   
   
   virtual void print () const;
   virtual void scale (float scaleFac);
   virtual float computeArea () const;
   virtual float computePerimeter () const;
   virtual void draw (easygl* window) const;
   virtual bool pointInside (float x, float y) const;
};

#endif	/* CIRCLE_H */
