#ifndef BODY_H
#define BODY_H

#include <QString>

#define G (6.67428e-11) // Units are m3 kg-1 s-2 (gravitational constant)
#define AU (1.4960000e+11) // Units are km (astronomical unit)

/*
 * A base class for any generic celestial body. Assumes body is at least
 * two-dimensional.
 */
class Body
{
public:
    Body(QString name = "",
         QString colour = "#ffffff",
         double mass = 0,
         double radius = 0,
         double xPosition = 0,
         double yPosition = 0,
         double xVelocity = 0,
         double yVelocity = 0) :
        m_name(name),
        m_colour(colour),
        m_mass(mass),
        m_radius(radius),
        m_xPosition(xPosition),
        m_yPosition(yPosition),
        m_xVelocity(xVelocity),
        m_yVelocity(yVelocity) { }
    virtual ~Body();

    // Getters
    QString getName() const { return m_name; }
    QString getColour() const { return m_colour; }
    double getMass() const { return m_mass; }
    double getRadius() const { return m_radius; }
    double getXPosition() const { return m_xPosition; }
    double getYPosition() const { return m_yPosition; }
    double getXVelocity() const { return m_xVelocity; }
    double getYVelocity() const { return m_yVelocity; }

    // Setters
    void setName(const QString name) { m_name = name; }
    void setColour(const QString colour) { m_colour = colour; }
    void setMass(const double mass) { m_mass = mass; }
    void setRadius(const double radius) { m_radius = radius; }
    void setXPosition(const double xPosition) { m_xPosition = xPosition; }
    void setYPosition(const double yPosition) { m_yPosition = yPosition; }
    void setXVelocity(const double xVelocity) { m_xVelocity = xVelocity; }
    void setYVelocity(const double yVelocity) { m_yVelocity = yVelocity; }

protected:
    QString m_name;
    QString m_colour; // Colour as a hexcode
    double m_mass; // Mass in kg
    double m_radius; // Radius in km
    double m_xPosition; // Units are km
    double m_yPosition; // Units are km
    double m_xVelocity; // Units are m/s
    double m_yVelocity; // Units are m/s
};

#endif // BODY_H
