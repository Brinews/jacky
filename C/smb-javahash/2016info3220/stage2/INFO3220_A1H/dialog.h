#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPainter>
#include <QTimer>

/*
 * Code derived from Week 3 tutorial.
 */
namespace Ui { // Should this be here?
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    // Calls all simulation update steps before each time step
    void nextFrame();

protected:
    // Renders simulation results to the Dialog
    void paintEvent(QPaintEvent *event);

    // Commences and connects animation with a QTimer
    void startAnimation();

private:
    Ui::Dialog* ui;
    int m_timestep;
};

#endif // DIALOG_H
