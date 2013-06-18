#include "simulation.h"
#include "QDebug"
#include "math.h"

simulation::simulation()
{
//start_ausrichtung=0;
    schub[0]=0;
    schub[1]=0;
}

void simulation::sim()
{
    double alpha;
    if (xList.size()!=0){
        qDebug()<<"if";
        start_x = xList.at(0);
        start_y = yList.at(0);
        //start_ausrichtung = 0;
        //start_z = zList.at(0);
    }
    else {
        scheitel=100;
        start_ausrichtung=71.79;
        //qDebug() << "else";
        start_x = 1000;
        start_y = -800;
        //start_ausrichtung = (start_ausrichtung*PI)/180;
        pos1x=start_x+cos(((start_ausrichtung-90)*PI)/180)*(scheitel/2);
        pos1y=start_y+sin(((start_ausrichtung-90)*PI)/180)*(scheitel/2);
        //qDebug() << "Position1: x: "<<pos1x<<" y: "<<pos1y;
        pos2x=start_x+cos(((start_ausrichtung+90)*PI)/180)*(scheitel/2);
        pos2y=start_y+sin(((start_ausrichtung+90)*PI)/180)*(scheitel/2);
        //qDebug() << "Position2: x: "<<pos2x<<" y: "<<pos2y;
        //start_z = 0;
    }


    //if (modus){
        rechteDistanz=(schub[0]*SIMULATIONMAX)/100;
        linkeDistanz=(schub[1]*SIMULATIONMAX)/100;
        buf1=rechteDistanz-linkeDistanz;
        buf1=buf1/scheitel;
        //qDebug()<<buf1;
        alpha = (atan(buf1)*180)/PI;
        //qDebug()<<alpha;
        start_ausrichtung+=alpha;
        pos1x += cos((start_ausrichtung*PI)/180)*rechteDistanz;
        pos1y += sin((start_ausrichtung*PI)/180)*rechteDistanz;
        pos2x += cos((start_ausrichtung*PI)/180)*linkeDistanz;
        pos2y += sin((start_ausrichtung*PI)/180)*linkeDistanz;
        //qDebug() << "Position1: x: "<<pos1x<<" y: "<<pos1y;
        posx=pos1x+0.5*(pos2x-pos1x);
        posy=pos1y+0.5*(pos2y-pos1y);

qDebug() << "Position_Zeppelin: x: "<<posx<<" y: "<<posy;
    xList.prepend(posx);
    yList.prepend(posy);
}
