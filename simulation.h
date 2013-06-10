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
    int start_x;
    int start_y;
    int start_ausrichtung;

};

#endif // SIMULATION_H
