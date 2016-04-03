#ifndef GLMODEL_H
#define GLMODEL_H

#include <QGLWidget>
#include <GL/glut.h>

#include "sphere.h"
#include "planet.h"

class glmodel : public QGLWidget
{
public:
    glmodel();
    glmodel(QGLWidget *parent): QGLWidget(parent) {
         setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    }

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void drawModel(Planet *);

    Sphere spIns;
};

#endif // GLMODEL_H
