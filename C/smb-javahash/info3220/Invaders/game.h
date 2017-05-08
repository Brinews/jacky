#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "defaults.h"
#include "spaceship.h"
#include "alien.h"

#include <QDialog>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

/**
 * @brief The Game class
 * Contains the game "level"
 */
class Game : public QDialog
{
    Q_OBJECT

    Config *conf;
    Spaceship *ship;
    Alien *alien;
    int score;

    QTimer *timer;

public:
    Game();
    virtual ~Game();

    Config *getConf() { return conf; }

    void setConf(Config *c) { conf = c; }
    void setShip(Spaceship *s) { ship = s; }
    void setAlien(Alien *a) { alien = a; }

    void startTimer(int tick) { timer->start(tick); }

protected:
    void paintEvent(QPaintEvent *event);
public slots:
    void nextFrame();
};

#endif // GAME_H
