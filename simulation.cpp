#include "simulation.h"
#include "QDebug"
#include "math.h"

simulation::simulation()
{
//start_ausrichtung=0;
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
    double alpha,theta,gamma,tau,z,b,h;
    if (xList.size()!=0){
        qDebug()<<"if";
        start_x = xList.at(0);
        start_y = yList.at(0);
        //start_ausrichtung = 0;
        //start_z = zList.at(0);
    }
    else {
        scheitel=100;
        start_ausrichtung=180;
        qDebug() << "else";
        start_x = 2000;
        start_y = 2000;
        //start_ausrichtung = (start_ausrichtung*PI)/180;

        //start_z = 0;
    }

    rechteDistanz=(schub[1]*SIMULATIONMAX);
    linkeDistanz=(schub[0]*SIMULATIONMAX);
    alpha = start_ausrichtung*PI/180;

    if (rechteDistanz==linkeDistanz)
    {
        qDebug()<<start_ausrichtung;
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

qDebug() << "Position_Zeppelin: x: "<<posx<<" y: "<<posy;
    xList.prepend(posx);
    yList.prepend(posy);
}
