#include "simulation.h"
#include "math.h"

void sim::Simulation()
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

    posx=start_x+cos((start_ausrichtung*PI)/180)*(schub[0]/100)*SIMULATIONMAX;
    posy=start_y+sin((start_ausrichtung*PI)/180)*(schub[1]/100)*SIMULATIONMAX;

    xList.prepend(posx);
    yList.prepend(posy);
}
