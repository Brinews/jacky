
#include <QApplication>
#include <QString>
#include <QVector>
#include <iostream>

#include "dialog.h"
#include "engine.h"
#include "component2dfactory.h"

int main(int argc, char *argv[])
{
    QString filepath("config.txt");

    // Setup application
    QApplication app(argc, argv);
    Dialog dialog;
    ComponentFactory& factory = *(new Component2DFactory); // 2D
    Engine::getInstance()->parseConfigFile(filepath, factory);

    // Start rendering
    dialog.show();
    return app.exec();
}
