#include <QApplication>
#include "mainwindow.h"

#include "sphere.h"
#include "planet.h"

#include "glmodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //MainWindow w;
    //w.show();

    glmodel gl;
    gl.show();
    
    return a.exec();
}
