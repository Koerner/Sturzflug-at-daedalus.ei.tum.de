#include "simulation.h"
#include "QDebug"
#include "math.h"

simulation::simulation()
{

}

void simulation::sim()
{
    if (xList.size()!=0){
        start_x = xList.at(0);
        start_y = yList.at(0);
        //start_z = zList.at(0);
    }
    else {
        start_x = 0;
        start_y = 0;
        //start_z = 0;
    }
    start_ausrichtung = 45;
    posx=start_x+(schub[0]*SIMULATIONMAX*cos((start_ausrichtung*PI)/180))/100;
    posy=start_y+(schub[1]*SIMULATIONMAX*sin((start_ausrichtung*PI)/180))/100;


    xList.prepend(posx);
    yList.prepend(posy);
}
