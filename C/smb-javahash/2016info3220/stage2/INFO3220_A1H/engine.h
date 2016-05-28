#ifndef ENGINE_H
#define ENGINE_H

#include <QVector>
#include <QString>

#include "body.h"
#include "componentfactory.h"

/*
 * Manages physics simulation and body generation. Designed as a Singleton.
 */
class Engine
{
public:
    virtual ~Engine();
    static Engine* getInstance();

    void update();

    /*
     * Calls upon BodyFactory to generate bodies which is then stored.
     * If an error is discovered while parsing, an error is logged and program
     * will exit.
     */
    void parseConfigFile(const QString filepath,
                         const ComponentFactory& factory);

    QVector<Body>* getBodies() const { return m_bodies; }

private:
    Engine();
    QVector<Body>* m_bodies;
    static Engine* m_instance;
    static bool m_exists;
};

#endif // ENGINE_H
