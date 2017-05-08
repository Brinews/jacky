#include "gamebody.h"
#include "config.h"
#include <QDebug>
#include <QGradient>

gameBall::gameBall(
        gameComponentType type,
        const std::string& name,
        const std::string& parentName)
        : gameComponent(type, name, parentName)
        , m_xVelocity(0.0)
        , m_yVelocity(0.0)
        , m_xPosition(0.0)
        , m_yPosition(0.0)
        , m_color(Qt::white)
        , m_size(small)
{ }

void gameBall::render(QPainter& painter) const
{

    {
        //no outline
        painter.setPen(Qt::NoPen);

        //gradient brush
        QRadialGradient gradient(m_xPosition, m_yPosition, 3*2);
        gradient.setColorAt(0.25, Qt::white);

        painter.setBrush(gradient);

        double scale = 1.0;

        if (m_size == middle) {
            scale = 1.0;
        } else if (m_size == big) {
            scale = 1.5;
        }

        painter.drawRect(m_xPosition+10, m_yPosition, 6*scale, 10*scale);
    }
}

void gameBall::renderLabel(QPainter& painter) const
{
}

void gameBall::changeColor()
{
}

void gameBall::changeSize()
{
    m_size = (BodySize)(((int)m_size+1)%3);
}

void gameBall::updatePosition(int timestep)
{
     m_yPosition += -2;
}

