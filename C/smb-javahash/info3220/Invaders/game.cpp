#include "game.h"

#include <iterator>
#include <iostream>

Game::Game()
{
    score = 0;
    // set background to black
    setStyleSheet("background-color: #000000;");

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
}

Game::~Game()
{
    delete conf;
    delete ship;
    delete alien;
    delete timer;
}

void Game::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // draw spaceship
    painter.drawPixmap(ship->getX(), ship->getY(), ship->getPixmap());

    // draw each laser beam
    std::list<LaserBeam*> lbst = ship->getBeams();
    std::list<LaserBeam*>::const_iterator itr;
    for (itr = lbst.begin(); itr != lbst.end(); itr++) {
    //for (LaserBeam *b : ship->getBeams()) {
        //painter.drawPixmap(b->getX(), b->getY(), b->getPixmap());
        //std::cout << (*itr)->getX() << std::endl;
        painter.drawPixmap((*itr)->getX(), (*itr)->getY(), (*itr)->getPixmap());
    }

    if (alien != NULL) {

        painter.drawPixmap(alien->getX(), alien->getY(), alien->getPixmap());

        //std::cout << "--"<< alien->getY() << std::endl;

        // draw each laser beam
        std::list<LaserBeam*> lbst1 = alien->getBeams();
        std::list<LaserBeam*>::const_iterator it;
        for (it = lbst1.begin(); it != lbst1.end(); it++) {
            painter.drawPixmap((*it)->getX(), (*it)->getY(), (*it)->getPixmap());
        }
    }

}

void Game::nextFrame() {
    // execute next action (left, right, shoot) from config
    char action = conf->nextAction();

    if (action == LEFT) {
        ship->moveLeft();
    } else if (action == RIGHT) {
        ship->moveRight();
    } else if (action == SHOOT) {
        ship->shoot();
    }

    if (alien != NULL) {
        alien->shoot();
        alien->updateTimer(TICK);
        alien->animateBeams();
    }

    // animate laser beams from spaceship
    ship->animateBeams();


    if (alien != NULL && alien->collision(ship)) { // ship was hit
        // game over
        cout << "space hit, Game over\n";
        update();
        delete timer;
    }

    if (alien != NULL && ship->collision(alien)) { // alien was hit
        delete alien;
        alien = NULL;
        score += 10; // accumulate score
        cout << "alien hit, score=" << score << endl;
    }

    update();
}
