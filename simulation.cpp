#include "simulation.h"
#include "QDebug"
#include "math.h"

simulation::simulation()
{
    schub[0]=0;
    schub[1]=0;
}

int simulation::Runden(double Zahl)
{
    if (Zahl>=0){Zahl += 0.5;}
    else {Zahl -= 0.5;}
    return Zahl;
}

void simulation::sim()
{
    scheitel=400;
    double alpha,theta,gamma,tau,z,b,h;
    if (xList.size()!=0){
        start_x = xList.at(0);
        start_y = yList.at(0);
        //start_z = zList.at(0);
    }
    else {
        //scheitel=100;
        start_ausrichtung=0;
        start_x = 0;
        start_y = 0;
        //start_z = 0;
    }

    if (schub[0]<0&&schub[1]<0)
    {}
    else
    {
    rechteDistanz=(schub[1]*SIMULATIONMAX);
    linkeDistanz=(schub[0]*SIMULATIONMAX);
    alpha = start_ausrichtung*PI/180;

    if (rechteDistanz==linkeDistanz)
    {
        posx=Runden(start_x+cos(alpha)*rechteDistanz/100);
        posy=Runden(start_y+sin(alpha)*rechteDistanz/100);
    }
    else
    {
        if (rechteDistanz<linkeDistanz)
        {
            Radius=((rechteDistanz+linkeDistanz)/(linkeDistanz-rechteDistanz))*scheitel;
            theta=(linkeDistanz/(Radius+scheitel)/100);
            start_ausrichtung+=(theta*180/PI);
        }
        else
        {
            Radius=((rechteDistanz+linkeDistanz)/(rechteDistanz-linkeDistanz))*scheitel;
            theta=(linkeDistanz/(Radius-scheitel)/100);
            start_ausrichtung-=(theta*180/PI);
        }
        z=Radius*(1-cos(theta));
        h=Radius*sin(theta);
        b=sqrt(z*z+h*h);
        gamma=atan(z/h);
        tau=alpha+theta-gamma;
        posx=Runden(start_x+b*cos(tau));
        posy=Runden(start_y+b*sin(tau));
    }

    xList.prepend(posx);
    yList.prepend(posy);
    //Ausrichtung kalibrieren
    while(start_ausrichtung>180)
    {
        start_ausrichtung=start_ausrichtung-360;
    }
    while (start_ausrichtung<=-180)
    {
        start_ausrichtung=start_ausrichtung+360;
    }
    qDebug()<< "Position Zeppelin: "<<xList.at(0)<<"(x),"<<yList.at(0)<<"(y)";
    qDebug()<<"Ausrichtung:"<<start_ausrichtung;
    }
}
