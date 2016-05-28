#ifndef COMPONENT2DFACTORY_H
#define COMPONENT2DFACTORY_H

#include "componentfactory.h"
#include "definition.h"
#include "body2d.h"

/*
 * Manages construction of two-dimensional game components.
 * Designed as a Concrete Factory implementation of ComponentFactory.
 */
class Component2DFactory : public ComponentFactory
{
public:
    Component2DFactory();
    virtual ~Component2DFactory();

    Body& createBody(const Definition def) const;
};

#endif // COMPONENT2DFACTORY_H
