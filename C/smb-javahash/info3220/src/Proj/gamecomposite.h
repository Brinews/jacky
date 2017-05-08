#ifndef gameCOMPOSITE_H
#define gameCOMPOSITE_H

#include "gamecomponent.h"
#include <string>
#include <vector>


class gameComposite : public gameComponent {

public:
    gameComposite(
            gameComponentType type,
            const std::string& name,
            const std::string& parentName = "");
    virtual ~gameComposite();

    //build up the composition
    virtual void add(gameComponent* component) { m_children.push_back(component); }

    /*********************************************
     * Inherited methods from gameComponent
     * *******************************************/
    virtual void render(QPainter& painter) const;
    virtual void renderLabel(QPainter& painter) const;

    virtual void updatePosition(int timestep);
    virtual void changeSize();
    virtual void changeColor();

    /*********************************************
     * Methods used to enable construction of the game with relative positions
     * *******************************************/
    void setPosition(double x, double y) { m_xPosition = x; m_yPosition = y; }
    void setVelocity(double x, double y) { m_xVelocity = x; m_yVelocity = y; }

    void convertRelativeToAbsolute(double xp, double yp, double xv, double yv);

private:
    std::vector<gameComponent*> m_children;

    //used only to enable construction of the game with relative positions
    double m_xVelocity;
    double m_yVelocity;
    double m_xPosition;
    double m_yPosition;

};

#endif // gameCOMPOSITE_H
