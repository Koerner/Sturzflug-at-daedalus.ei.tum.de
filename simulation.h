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
    double spannweite;
    int Runden(double Zahl);

private:
    double posx;
    double posy;
    double Radius;
    int pos1x;
    int pos1y;
    int pos2x;
    int pos2y;
    double buf1;
    //double buf2;
    double start_x;
    double start_y;
    double start_ausrichtung;
    double rechteDistanz;
    double linkeDistanz;
    double scheitel;

};

#endif // SIMULATION_H
