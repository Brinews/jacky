#include "dialog.h"
#include "ui_dialog.h"
#include "engine.h"
#include "body2d.h"
#include <QDebug>

#define DIALOG_WIDTH (600)
#define DIALOG_HEIGHT (600)

#define X_CENTER (DIALOG_WIDTH/2) // screen x-center
#define Y_CENTER (DIALOG_HEIGHT/2) // screen y-center

#define ORBIT_SCALE (9.0 / AU) // scaling constant for rendering bodies on screen
#define TRAVEL_RATE (3e+5)

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_timestep(16)
{
    // Setup dialog window
    ui->setupUi(this);
    this->update();
    this->resize(DIALOG_WIDTH, DIALOG_HEIGHT);
    this->setStyleSheet("background-color: #000;");

    // Start animation
    this->startAnimation();
}

Dialog::~Dialog()
{

}

void Dialog::startAnimation()
{
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    timer->start(m_timestep);
}

void Dialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Render all bodies
    QVector<Body>* bodies = Engine::getInstance()->getBodies();
    for (int i = 0; i < bodies->size(); i++)
    {
        Body& b = (*bodies)[i];
        QBrush brush(QColor(b.getColour()));
        painter.setBrush(brush);
        painter.drawEllipse(X_CENTER-b.getXPosition()*ORBIT_SCALE-b.getRadius(),
                            Y_CENTER-b.getYPosition()*ORBIT_SCALE-b.getRadius(),
                            b.getRadius()*2,
                            b.getRadius()*2);
    }
}

void Dialog::nextFrame()
{
    // Add attractions to bodies
    QVector<Body>& bodies = *(Engine::getInstance()->getBodies());
    QVector<double> totalForces(bodies.size() * 2);
    for (int i = 0; i < bodies.size(); i++)
    {
        Body2D& self = static_cast<Body2D&>(bodies[i]);
        for (int j = 0; j < bodies.size(); j++)
        {
            Body2D & other = static_cast<Body2D&>(bodies[j]);
            if (self.getName() != other.getName()) // Exclude self
            {
                self.addAttraction(other, totalForces[2*i], totalForces[2*i+1]);
            }
        }
    }

    // Update acceleration vector of bodies
    for (int i = 0; i < bodies.size(); i++)
    {
        Body2D& body = static_cast<Body2D&>(bodies[i]);
        body.update(totalForces[2*i], totalForces[2*i+1], TRAVEL_RATE);
    }

    update();
}
