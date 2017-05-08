#include "gamebody.h"
#include "config.h"
#include <QDebug>
#include <QGradient>

gameBody::gameBody(
        gameComponentType type,
        const std::string& name,
        const std::string& parentName)
        : gameComponent(type, name, parentName)
        , m_xVelocity(0.0)
        , m_yVelocity(0.0)
        , m_xPosition(0.0)
        , m_yPosition(0.0)
        , m_color(Qt::white)
        , m_oldColor(Qt::blue)
        , m_size(small)
{
    myball = new gameBall(ball, "test", "Sol");
    myball->setPosition(0, 0);
    myball->setColor(Qt::blue);
}

void gameBody::render(QPainter& painter) const
{
    Config* config = Config::getInstance();

    //get scaled position and radius
    double x = m_xPosition / config->getDistanceScale();
    double y = m_yPosition / config->getDistanceScale();

    {
        //no outline
        painter.setPen(Qt::NoPen);

        //gradient brush
        QRadialGradient gradient(x, y, 3*2);
        gradient.setColorAt(0.25, m_color);

        painter.setBrush(gradient);

        double scale = 0.5;

        if (m_size == middle) {
            scale = 1.0;
        } else if (m_size == big) {
            scale = 1.5;
        }
        painter.drawRect(x, y, 50*scale, 16*scale);
        painter.drawRect(x+4*scale, y-6*scale, 42*scale, 6*scale);
        painter.drawRect(x+22*scale, y-16*scale, 6*scale, 10*scale);
        painter.drawRect(x+24*scale, y-20*scale, 2*scale, 4*scale);

        myball->render(painter);


    }
}

void gameBody::renderLabel(QPainter& painter) const
{
}

void gameBody::changeColor()
{
    QColor temp = m_color;
    m_color = m_oldColor;
    m_oldColor = temp;
}

void gameBody::changeSize()
{
    m_size = (BodySize)(((int)m_size+1)%3);
}

void gameBody::updatePosition(int timestep)
{
    Config* config = Config::getInstance();

    if (timestep == 0) { // move up
        m_yPosition -= 2*config->getDistanceScale();
        return;
    }

    if (timestep == -1) { // move left
        m_xPosition -= 2*config->getDistanceScale();
        return;
    }

    if (timestep == -2) { // move right
        m_xPosition += 2*config->getDistanceScale();
        return;
    }

    if (timestep == -3) { // shot a ball
        myball->setPosition(m_xPosition/config->getDistanceScale(), m_yPosition/config->getDistanceScale());
        return;
    }

     if (m_xPosition / config->getDistanceScale() > 400) {
         m_xPosition = 0;
     }

     m_xPosition += m_xVelocity * timestep;

     if (m_yPosition / config->getDistanceScale() > 400) {
        m_yPosition = 0;
     }

     m_yPosition += m_yVelocity * timestep;

     myball->updatePosition(timestep);
}

