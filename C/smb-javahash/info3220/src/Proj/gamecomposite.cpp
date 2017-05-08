#include "gamecomposite.h"

gameComposite::gameComposite(
        gameComponentType type,
        const std::string& name,
        const std::string& parentName)
        : gameComponent(type, name, parentName)
        , m_xVelocity(0.0)
        , m_yVelocity(0.0)
        , m_xPosition(0.0)
        , m_yPosition(0.0)
{ }

gameComposite::~gameComposite()
{
    for (int i = 0; i < m_children.size(); i++) {
        gameComponent *child = m_children[i];
    //for(gameComponent* child : m_children) {
        delete child;
    }
}


//render the subtree
void gameComposite::render(QPainter& painter) const
{
    //for(gameComponent* child : m_children)
    for (int i = 0; i < m_children.size(); i++)
    {
        gameComponent *child = m_children[i];
        child->render(painter);
    }
}

void gameComposite::renderLabel(QPainter& painter) const
{
    //for(gameComponent* child : m_children)
    for (int i = 0; i < m_children.size(); i++)
    {
        gameComponent *child = m_children[i];
        child->renderLabel(painter);
    }
}

//update the positions of all components of this object
void gameComposite::updatePosition(int timestep)
{
    //for(gameComponent* child : m_children)
    for (int i = 0; i < m_children.size(); i++)
    {
        gameComponent *child = m_children[i];
        child->updatePosition(timestep);
    }
}

void gameComposite::changeColor()
{
    for (int i = 0; i < m_children.size(); i++)
    {
        gameComponent *child = m_children[i];
        child->changeColor();
    }
}

void gameComposite::changeSize()
{
    for (int i = 0; i < m_children.size(); i++)
    {
        gameComponent *child = m_children[i];
        child->changeSize();
    }
}

void gameComposite::convertRelativeToAbsolute(double xp, double yp, double xv, double yv)
{
    xp += m_xPosition;
    yp += m_yPosition;
    xv += m_xVelocity;
    yv += m_yVelocity;

    //for(gameComponent* child : m_children)
    for (int i = 0; i < m_children.size(); i++)
    {
        gameComponent *child = m_children[i];
        child->convertRelativeToAbsolute(xp, yp, xv, yv);
    }
}

