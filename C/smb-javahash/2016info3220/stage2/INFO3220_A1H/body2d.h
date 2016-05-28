#ifndef BODY2D_H
#define BODY2D_H

#include "body.h"

/*
 * A two-dimensional representation of a celestial body. Inherits from Body.
 */
class Body2D : public Body
{
public:
    Body2D(QString name = "",
         QString colour = "#ffffff",
         double mass = 0,
         double radius = 0,
         double xPosition = 0,
         double yPosition = 0,
         double xVelocity = 0,
         double yVelocity = 0) :
        Body(name,
             colour,
             mass,
             radius,
             xPosition,
             yPosition,
             xVelocity,
             yVelocity) { }
    virtual ~Body2D();

    // Returns Euclidean distance to another body
    double getDistance(const Body2D &other) const;

    // Adds an additional exerted force, caused by another body
    void addAttraction(const Body2D &other, double &xForce, double &yForce);

    // Updates velocity and position by total force for one time step
    void update(double xTotalForce, double yTotalForce, double timestep);
};

#endif // BODY2D_H
