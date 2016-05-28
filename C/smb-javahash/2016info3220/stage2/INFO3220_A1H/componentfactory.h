#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <QDebug>
#include <QString>
#include "definition.h"
#include "body.h"

/*
 * Manages construction of generic game components.
 * Designed with Abstract Factory pattern and implements several
 * Factory Methods.
 */
class ComponentFactory
{
public:
    ComponentFactory();
    virtual ~ComponentFactory();

    // Factory method for generating a Definition
    virtual Definition& createDefinition(const QString className) const
        { return *(new Definition(className)); }

    // Factory method for generating a Body
    virtual Body& createBody(const Definition def) const
        { return *(new Body()); }
};

#endif // COMPONENTFACTORY_H
