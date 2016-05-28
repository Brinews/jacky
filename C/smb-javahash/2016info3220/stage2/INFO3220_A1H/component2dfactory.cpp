#include "component2dfactory.h"

#include <QDebug>

Component2DFactory::Component2DFactory()
{

}

Component2DFactory::~Component2DFactory()
{

}


Body& Component2DFactory::createBody(const Definition def) const
{
    Body2D* body = new Body2D();
    //for (QString key : def.getKeys())
    for (int i = 0; i < def.getKeys().length(); i++)
    {
        QString key = def.getKeys().at(i);

        if (key == QString("name"))
        {
            body->setName(def.getField(key));
        }
        else if (key == QString("colour"))
        {
            body->setColour(def.getField(key));
        }
        else if (key == QString("radius"))
        {
            body->setRadius(def.getField(key).toDouble());
        }
        else if (key == QString("mass"))
        {
            body->setMass(def.getField(key).toDouble());
        }
        else if (key == QString("xPosition"))
        {
            body->setXPosition(def.getField(key).toDouble());
        }
        else if (key == QString("yPosition"))
        {
            body->setYPosition(def.getField(key).toDouble());
        }
        else if (key == QString("xVelocity"))
        {
            body->setXVelocity(def.getField(key).toDouble());
        }
        else if (key == QString("yVelocity"))
        {
            body->setYVelocity(def.getField(key).toDouble());
        }
        else {
            qCritical() << "Error creating body, unexpected key"
                        << key;
            exit(1);
        }
    }
    return *body;
}
