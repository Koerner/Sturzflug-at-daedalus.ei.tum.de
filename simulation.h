#ifndef SIMULATION_H
#define SIMULATION_H


#include <QList>
#include "define.h"


class sim
{
public:
    void Simulation();

private:
    int posx;
    int posy;
    int start_x;
    int start_y;
    int start_ausrichtung;
    int schub[2];
    QList<int> xList;
    QList<int> yList;
};

#endif // SIMULATION_H
