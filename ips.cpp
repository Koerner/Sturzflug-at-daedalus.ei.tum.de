#include "ips.h"
#include <iostream>



//ips::ips()
//{
//}

//Extrahiert die Laufzeit aller Stationen
void ips::setdata(QString comdata)
{
    QString delimiterPattern("\n");
    QStringList comdatasplit = comdata.split(delimiterPattern);
    int rawtime[10];
    int i=0;
    for (i=0; i<10; i++)
        {
        //if(i<=comdatasplit.size()){
        bool ok;
        rawtime[i]=comdatasplit[i].mid(16,4).toInt(&ok,10); //mid(16,4) = Position 17-20 extrahieren / String in int umwandeln Basis 10
        setstationtime(i,rawtime[i]);
        //}
        //else {setstationtime(i,0);}
        }
}
//STOP Extrahiert die Laufzeit aller Stationen

//Speichert die letzten 10 Laufzeiten aller Stationen / 0 ist aktuell
void ips::setstationtime (int nr, int rawtime)
{
    int i=0;
    for (i=0; i<9; i++)
    {stationtime[nr][i+1]=stationtime[nr][i];}
    stationtime[nr][0]=rawtime;


}
// STOP Speichert die letzten 10 Laufzeiten aller Stationen

int ips::gettime (int station, int time)
{
    return stationtime[station][time];
}

// Filter
double ips::gettimef(int station)
{
    int i=0;
    double timef=0;
    int array[10];
    int filterzus=5;
    int filterstr=1;

    for(i=0;i<filterzus;i++)
        array[i] = ips::gettime(station, i);

    std::sort(array, array+filterzus);
    timef=0;

    for(i=filterstr;i<(filterzus-filterstr);i++)
            timef=timef+array[i];

    timef=timef/(filterzus-(filterstr*2));
    return timef;
}
//STOP Filter

int ips::aufruf_wrapper()
{
    multilat meinMultilat;
    int numstations =5;
    int i = 0;
    double posx=0;
    double posy=0;
    double posz=0;
    //    millis = int(round(time.time() * 1000))
    //for station in stations:
    //    if (millis - station[4]) < 1500:
    //        numstations += 1;
    if (numstations > 2){
        i = -1;
        double cstations[5][3]={{0}};
        double cradii[5]={0};
        double cstartpos [3];
        cstartpos[0]=posx;
        cstartpos[0]=posy;
        cstartpos[0]=posz;

        //alle Werte auf Null setzen
        //??cstartpos = cstarttype(posx, posy, posz);
        int cnn = numstations;
        for(i=0; i < numstations; i++)
        {
           // if (millis - station[4]) < 1500:
                //cstations[i][0] = meineBodenstation.GetStationen_x(i);
                //cstations[i][1] = meineBodenstation.GetStationen_y(i);
                //cstations[i][2] = meineBodenstation.GetStationen_z(i);
                cradii[i] = gettimef(i)*0.343;
                //cradii[1] = 100;
                //cradii[2] = 300;
                //cradii[3] = 400;
        }


        double x;
        double y;
        double z;
        double deltar;
        //multilat.wrapper.restype=c_double


        deltar = wrapper(cstations, cstartpos, cradii, cnn, &x, &y, &z);

        posx = x;
        posy = y;
        posz = z;
        cout<< posx<< posy<< posz<<endl;
    }
}
#define BASES 20	//Max Anz an Basen




//Anz der Basen
int n = 0;
//Koord der Basen
double base_x[BASES];
double base_y[BASES];
double base_z[BASES];
//Abstaende zu den Basen
double r[BASES];

double start_x = 10000;
double start_y = 10000;
double start_z = 10000;

double posx = 0;
double posy = 0;
double posz = 0;

double ips::wrapper(double start[3], double radius[], int nn, double *refx, double *refy, double *refz)
{
    int i=0;
    for (i=0;i<nn;i++) {
      base_x[i] = w.posStation[i][0];
      base_y[i] = w.posStation[i][1];
      base_z[i] = w.posStation[i][2];
      r[i] = radius[i];

    }
    n = nn;
    start_x = start[0];
    start_y = start[1];
    start_z = start[2];

    rechne();
    *refx = posx;
    *refy = posy;
    *refz = posz;
    double genauigkeit = 0.0;
    return genauigkeit;
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

        //x_neu = x - schritt*richtung[0];
        //y_neu = y - schritt*richtung[1];
        //z_neu = z - schritt*richtung[2];
        x_neu = x - schritt*richtung[0];
        y_neu = y - schritt*richtung[1];
        z_neu = z - schritt*richtung[2];
        i++;

        //auf Staionaritaet pruefen
        if (((x-x_neu)*(x-x_neu)+(y-y_neu)*(y-y_neu)+(z-z_neu)*(z-z_neu))<0.0001)
            break;
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
