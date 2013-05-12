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
