#ifndef gameBALL_H
#define gameBALL_H

#include "gamecomponent.h"

class gameBall : public gameComponent
{
public:
    gameBall(
            gameComponentType type,
            const std::string& name,
            const std::string& parentName = "");

    virtual ~gameBall() {}

    /*********************************************
     * Inherited methods from gameComponent
     * *******************************************/
    //render the subtree
    virtual void render(QPainter &painter) const;
    virtual void renderLabel(QPainter& painter) const;

    //update the positions of all components of this object
    virtual void updatePosition(int timestep);
    virtual void changeColor();
    virtual void changeSize();

    //convert the initial (relative) position and velocity, to an absolute one
    //by translating the position and velocity with the values provided
    void convertRelativeToAbsolute(double xp, double yp, double xv, double yv) {
        m_xPosition += xp;
        m_yPosition += yp;
        m_xVelocity += xv;
        m_yVelocity += yv;
    }


    /*********************************************
     * Accessor methods
     * *******************************************/
    const QColor& getColor() const { return m_color; }
    double getPositionX() const { return m_xPosition; }
    double getPositionY() const { return m_yPosition; }


    BodySize getSize() const { return m_size; }


    void setPosition(const double x, const double y) { m_xPosition = x; m_yPosition = y; }
    void setVelocity(const double x, const double y) { m_xVelocity = x; m_yVelocity = y; }

    void setColor(const QColor& color) { m_color = color; }

    void setSize(const BodySize& bs) { m_size = bs; }

private:

    double m_xVelocity;
    double m_yVelocity;
    double m_xPosition;
    double m_yPosition;

    QColor m_color;

    BodySize m_size;
};

#endif // gameBODY_H
