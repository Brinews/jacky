#ifndef DIALOG_H
#define DIALOG_H

#include "config.h"
#include "gamecomponent.h"
#include <QDialog>
#include <QTimer>
#include <list>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    virtual ~Dialog();

private slots:
    //prepare the next frame, called by the timer
    void nextFrame();
    //toggle the simultation's pause state
    void togglePause();
    //toggle rendering of Zodiacs
    void toggleColor();
    //toggle rendering of labels
    void toggleSize();
    // set timer
    void setTimerStep();

private:
    //method called when the window is being redrawn
    void paintEvent(QPaintEvent *event);
    //pause (or unpause) the simulation
    void pause(bool pause);
    //handle key presses
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Dialog* ui;
    QTimer* m_timer; //Timer object for triggering updates

    //buttons for UI
    QPushButton* m_buttonPause;
    QPushButton* m_buttonColor;
    QPushButton* m_buttonSize;
    QPushButton* m_buttonTimer;

    double m_scaleTime; // scale times

    int m_width; //width of the window
    int m_height; //height of the window
    bool m_paused; //is the simulation paused?
    bool m_renderZodiacs; //should Zodiacs be rendered?
    bool m_renderLabels; //should labels be rendered?
    long m_timestamp; //simulation time since simulation shipt
    gameComponent* m_game; //The game

    Config* m_config; //the singleton config instance

};

#endif // DIALOG_H
