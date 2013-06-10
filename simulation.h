#ifndef SIMULATION_H
#define SIMULATION_H


#include <QList>
#include "define.h"


class simulation
{
public:
    simulation();
    void sim();
    int schub[2];
    QList<int> xList;
    QList<int> yList;

private:
    double posx;
    double posy;
    double start_x;
    double start_y;
    double start_ausrichtung;

};

#endif // SIMULATION_H
