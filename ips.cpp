#include "ips.h"
#include "mainwindow.h"
#include <iostream>



//ips::ips()
//{
//}

//Extrahiert die Laufzeit aller Stationen
void ips::setdata(QString comdata)
{
    QString delimiterPattern("\n");
    QStringList comdatasplit = comdata.split(delimiterPattern);
    int i=0;
    for (i=0; i<comdatasplit.size(); i++)
        {

        if(comdatasplit[i].length()>0){
        if (comdatasplit[i].left(1).contains("t", Qt::CaseInsensitive))
            {if(comdatasplit[i].length()==13)
                {
//            setstationtime(0,3);
                bool ok;
                int s=0;
                s=comdatasplit[i].mid(1,2).toInt(&ok,10)-1;
                int t=0;
                comdatasplit[i].mid(3,10).toInt(&ok,10);
                if(ok){
                t=comdatasplit[i].mid(3,10).toInt(&ok,10);
                }
                setstationtime(s,t);
               }
            else{ //braucht es das? ich glaub nicht
                if(comdatasplit[i].length()>3)
                {
                    bool ok;
                    int s=0;
                    comdatasplit[i].mid(1,2).toInt(&ok,10);
                    if(ok)
                    {
                    s=comdatasplit[i].mid(1,2).toInt(&ok,10)-1;
                    }
                    else
                    {
//                    s=comdatasplit[i-1].mid(1,2).toInt(&ok,10);  //geht nicht
                    }
                    setstationtime(s,0); //keepstationtime()
                }
                else
                {
//                    bool ok;
                    int s=0;
//                    s=comdatasplit[i-1].mid(1,2).toInt(&ok,10); //geht nicht
                    setstationtime(s,0); //keepstationtime
                }


            }
            }
        }
    }
}
//STOP Extrahiert die Laufzeit aller Stationen

//Speichert die letzten 10 Laufzeiten aller Stationen / 0 ist aktuell
void ips::setstationtime (int nr, int rawtime)
{
    int i=0;
    for (i=8; i>0; i--)
    {stationtime[nr][i]=stationtime[nr][i-1];}
    stationtime[nr][0]=rawtime;


}
void ips::keepstationtime (int nr)
{
    int i=0;
    for (i=8; i>0; i--)
    {stationtime[nr][i]=stationtime[nr][i-1];}
    stationtime[nr][0]=stationtime[nr][1];


}
// STOP Speichert die letzten 10 Laufzeiten aller Stationen

int ips::gettime (int station, int time)
{
    return stationtime[station][time];
}

// Filter
int ips::gettimef(int station)
{
    int i=0;
    double timef=0;
    int array[10];
    int filterzus=5;
    int filterstr=1;

    for(i=0;i<filterzus;i++)
       { array[i] = stationtime[station][i];}

    std::sort(array, array+filterzus);
    timef=0;

    for(i=filterstr;i<(filterzus-filterstr);i++)
            {timef=timef+array[i];}

    timef=timef/(filterzus-(filterstr*2));
    return timef;
}
//STOP Filter




//Triang.


int ips::wrapper()
{
    QList<int> xList;
    //QList<int> yList;
    //QList<int> zList;
    int numstations =3;
    int i = 0;
    int hans;
    //    millis = int(round(time.time() * 1000))

    for (i=0;i<numstations;i++) {

        base_x[i] = posStation[i][0];
        base_y[i] = posStation[i][1];
        base_z[i] = posStation[i][2];

      //r[i] = gettimef(i)*0.343;
        r[0]=1728;
        r[1]=3508;
        r[2]=4960;

    }
    n = numstations;
    start_x = pos_x[0];
    start_y = pos_y[0];
    start_z = pos_y[0];

    hans=rechne();
    hans = hans+1;

    //for (i=0;999;i++)
    //{   pos_x[i+1]=pos_x[i];
    //    pos_y[i+1]=pos_y[i];
    //    pos_z[i+1]=pos_z[i];
    //}
    pos_x[0]=posx;
    pos_y[0]=posy;
    pos_z[0]=posz;
    int b;
    xList.append(posx);
    b=xList.at(0);
    //int genauigkeit = 1;
    return b;
}

int ips::rechne()
{
    //double *richtung;
    double richtung[3]={0};
    double schritt = 0.0;
    double x = start_x;
    double y = start_y;
    double z = start_z;
    double x_neu = 0.0;
    double y_neu = 0.0;
    double z_neu = 0.0;
    int i = 0;


    while(i<300)
    {
        //Grad (google it!!)
        gradf(x,y,z,n);
        richtung[0] = erg[0];
        richtung[1] = erg[1];
        richtung[2] = erg[2];
        //Schrittweite nach Armijo (google it!!)
        schritt = armijo(x,y,z,n);

        x_neu = x - schritt*richtung[0];
        y_neu = y - schritt*richtung[1];
        z_neu = z - schritt*richtung[2];
        i++;

        //auf Staionaritaet pruefen
        if (((x-x_neu)*(x-x_neu)+(y-y_neu)*(y-y_neu)+(z-z_neu)*(z-z_neu))<0.0001)
           { break;}
        x = x_neu;
        y = y_neu;
        z = z_neu;

    }

    x = x_neu;
    y = y_neu;
    z = z_neu;

    posx = x;
    posy = y;
    posz = z;


    int ret = 0;
    return ret;
}


//zu minimierende Funktion f
double ips::f(double x, double y, double z, int n)
{
    int i = 0;
    double erg3 = 0.0;
    double zwi = 0.0;

    for(i = 0; i < n; i++)
    {
        zwi = (x-base_x[i])*(x-base_x[i])+(y-base_y[i])*(y-base_y[i])+(z-base_z[i])*(z-base_z[i])-r[i]*r[i];
        erg3 += zwi*zwi;
    }
    return erg3;
}


//grad f
void ips::gradf(double x, double y, double z, int n)
{

    //erg[0] = 0.0; erg[1] = 0.0; erg[2] = 0.0;
    int i = 0;
    double zwi = 0.0;
    //x-Komp
    for (i = 0; i < n; i++)
    {
        zwi = (x-base_x[i])*(x-base_x[i])+(y-base_y[i])*(y-base_y[i])+(z-base_z[i])*(z-base_z[i])-r[i]*r[i];
        erg[0] += zwi * (x-base_x[i]);
        erg[1] += zwi * (y-base_y[i]);
        erg[2] += zwi * (z-base_z[i]);

    }

    erg[0] = erg[0] *4;
    erg[1] = erg[1] *4;
    erg[2] = erg[2] *4;


    //return 0;
}


//armijo-Schrittweite bestimmen
double ips::armijo(double x, double y, double z, int n)
{
    double schritt = 1;
    int i = 0;
    double ls = 0.0;
    double rs = 0.0;
    //double *gradient; Bärle
    //gradient = grad_f(x,y,z,n); Bärle
    double gradient[3];
    gradient[0] = erg[0];
    gradient[1] = erg[1];
    gradient[2] = erg[2];


    while (i < 1200)
    {
        ls = f(x+schritt*(-gradient[0]),y+schritt*(-gradient[1]),z+schritt*(-gradient[2]),n);
        rs = f(x,y,z,n) + 0.5*schritt*(-gradient[0]*gradient[0]-gradient[1]*gradient[1]-gradient[2]*gradient[2]);
        if (ls <= rs)
            break;
        schritt = schritt / 2;
        i++;
    }

    return schritt;
}
