#include "glmodel.h"
#include "sphere.h"
#include "planet.h"

glmodel::glmodel()
{
    spIns.addPlanets("../sphere/main.config");
}

void glmodel::initializeGL()
{
    qglClearColor(Qt::black);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void glmodel::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(width) / height;
    glFrustum(-x, +x, -1.0, +1.0, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void glmodel::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    renderText(10,20,"Hello,My Sphere");


    glColor3f(1.0, 1.0, 0.0);


    glTranslatef(-1.5, 0.0, -6.0);
    glRotatef(9, 0.0, 0.0, 1.0);

    vector<Planet *> list = spIns.getPlanets();

    char buf[100];
    sprintf(buf, "Stars Num=%d", list.size());

    for (int i = 0; i < list.size(); i++) {
        drawModel(list[i]);
    }

    renderText(10, 100, buf);
}

void glmodel::drawModel(Planet *p)
{

    glTranslatef(p->getPosX(), p->getPosY(), p->getPosZ());

    glPushMatrix();
        glRotatef( -90, 1.0, 0.0, 0.0 );
        glColor3f(p->getColorR(), p->getColorG(),
                  p->getColorB());
        glutWireSphere(p->getSize(), 10, 10 );
        //glColor3f( 1.0, 1.0, 1.0);
    glPopMatrix();
}

void glmodel::mousePressEvent(QMouseEvent *event)
{
    //int lastPos = event->pos;
}

void glmodel::mouseMoveEvent(QMouseEvent *event)
{
    /*
    GLfloat dx = GLfloat( event->x() - lastPos.x( ) ) / width( ) ;
    GLfloat dy = GLfloat( event->y( ) - lastPos.y( ) ) / height( ) ;
    if( event->buttons() & Qt::LeftButton )
    {
        rotationX += 180 * dy ;
        rotationY += 180 * dx ;
        updateGL( ) ;                       // 如果是重载 paintEvent，则使用 update( ) ;
    }
    else if ( event->buttons() & Qt::RightButton )
    {
        rotationX += 180 * dy ;
        rotationZ += 180 * dx ;
        updateGL( ) ;                      // 如果是重载 paintEvent，则使用 update( ) ;
    }
    lastPos = event->pos( ) ;
    */
}
