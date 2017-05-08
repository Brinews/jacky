#ifndef LASERBEAM_H
#define LASERBEAM_H

#include <QDialog>
#include <QPainter>
#include <QPixmap>

/**
 * @brief The LaserBeam class
 * Represents a laser beam
 */
class LaserBeam
{
    QPixmap pic; // image of beam
    int x, y; // x, y coordinates of beam
    int s; // stride taken by beam at each tick of the timer

public:
    LaserBeam(int x, int y);
    virtual ~LaserBeam() {}

    // moves the beam along, called at each tick of the timer
    // returns true iff beam still exists in game window
    virtual bool move();

    void setS(int _s) { s = _s; }

    int getX() const { return x; }
    int getY() const { return y; }
    int getS() const { return s; }
    const QPixmap &getPixmap() const { return pic; }
};

#endif // LASERBEAM_H
