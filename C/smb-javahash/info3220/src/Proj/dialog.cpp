#include "dialog.h"
#include "ui_dialog.h"

#include "gamecomposite.h"
#include <QKeyEvent>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QInputDialog>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , m_width(800)
    , m_height(800)
    , m_paused(false)
    , m_renderZodiacs(true)
    , m_renderLabels(true)
    , m_timestamp(0)
    , m_scaleTime(1.0)
    , m_config(Config::getInstance())
{
    m_config->read("config.txt");
    m_game = m_config->parsegameBlocks();

    m_game->convertRelativeToAbsolute(0,0,0,0);

    //set backdrop to black
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);

    //make the window appear
    ui->setupUi(this);
    this->resize(m_width, m_height);

    //create the buttons
    m_buttonPause = new QPushButton("Pause", this);
    m_buttonColor = new QPushButton("ColorChange", this);
    m_buttonSize = new QPushButton("SizeChange", this);
    m_buttonTimer = new QPushButton("TimerStep", this);

    m_buttonPause->setGeometry(QRect(QPoint(0, 0), QSize(100, 50)));
    m_buttonColor->setGeometry(QRect(QPoint(100, 0), QSize(100, 50)));
    m_buttonSize->setGeometry(QRect(QPoint(200, 0), QSize(100, 50)));
    m_buttonTimer->setGeometry(QRect(QPoint(300,0), QSize(100,50)));

    connect(m_buttonPause, SIGNAL(released()), this, SLOT(togglePause()));
    connect(m_buttonColor, SIGNAL(released()), this, SLOT(toggleColor()));
    connect(m_buttonSize, SIGNAL(released()), this, SLOT(toggleSize()));
    connect(m_buttonTimer, SIGNAL(released()), this, SLOT(setTimerStep()));

    //setup timer
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    pause(false);
}

Dialog::~Dialog()
{
    delete ui;
    delete m_timer;
    delete m_buttonPause;
    delete m_buttonColor;
    delete m_buttonSize;
    delete m_buttonTimer;
    delete m_game;

}

void Dialog::setTimerStep()
{
    bool ok;
    int i = QInputDialog::getInt(this, tr("Set Timer Step(msec)"),
                                      tr("msecond:"), 25, 0, 100, 1, &ok);
    if (ok)
        m_timer->setInterval(i);
}

void Dialog::toggleColor()
{
    m_game->changeColor();
}

void Dialog::toggleSize()
{
    m_game->changeSize();
}

void Dialog::togglePause()
{
    pause(!m_paused);
}

void Dialog::pause(bool pause)
{
    if(pause)
    {
        m_timer->stop();
        m_paused = true;
    }
    else
    {
        /* we can change the timer here too */
        m_timer->start(1000 / m_config->getFramesPerSecond());
        m_paused = false;
    }
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Space:
        pause(!m_paused);
        return;
    case Qt::Key_A:
        m_game->updatePosition(-1);
        break;
    case Qt::Key_D:
        m_game->updatePosition(-2);
        break;
    case Qt::Key_W:
        m_game->updatePosition(0);
        break;
    case Qt::Key_X:
        m_game->updatePosition(-3);
        break;
    default:
        return;
    }
}

void Dialog::nextFrame()
{

    int step = m_config->getPhysicsStepSize() / m_config->getOvercalculatePhysicsAmount();
    for(int i = 0; i < m_config->getPhysicsStepSize(); i += step)
    {
        //update physics
        m_game->updatePosition(step);
        //some time has passed
        m_timestamp += step;
    }

    //update the window (this will trigger paintEvent)
    update();
}

void Dialog::paintEvent(QPaintEvent *event)
{
    //suppress 'unused variable' warning
    Q_UNUSED(event);

    //redraw the game
    QPainter painter(this);

    //offset the painter so (0,0) is the center of the window
    painter.translate(m_width/2, m_height/2);

    m_game->render(painter);

    if(m_renderLabels)
    {
        m_game->renderLabel(painter);
    }
}




