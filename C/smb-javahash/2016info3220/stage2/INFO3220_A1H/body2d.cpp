#include <cmath>
#include "body2d.h"

Body2D::~Body2D()
{

}

double Body2D::getDistance(const Body2D &other) const
{
    double xDelta = other.getXPosition() - m_xPosition;
    double yDelta = other.getYPosition() - m_yPosition;
    double distance = sqrt(xDelta * xDelta + yDelta * yDelta);
    return distance;
}

void Body2D::addAttraction(const Body2D &other, double &xForce, double &yForce)
{
    double distance = getDistance(other);
    double xDelta = other.getXPosition() - m_xPosition;
    double yDelta = other.getYPosition() - m_yPosition;
    double theta = atan2(yDelta, xDelta);
    double force = G * m_mass * other.getMass() / (distance * distance);
    xForce += force * cos(theta);
    yForce += force * sin(theta);
}

void Body2D::update(double xTotalForce, double yTotalForce, double timestep)
{
    // Calculate acceleration
    double xAccel = xTotalForce / m_mass ;
    double yAccel = yTotalForce / m_mass ;

    // Integrate acceleration
    m_xVelocity += xAccel * timestep;
    m_yVelocity += yAccel * timestep;

    // Integrate velocity
    m_xPosition += m_xVelocity * timestep;
    m_yPosition += m_yVelocity * timestep;
}

