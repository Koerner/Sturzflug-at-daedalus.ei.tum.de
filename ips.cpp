#include "ips.h"
#include <iostream>



//ips::ips()
//{
//}

void ips::setdata(QString comdata)
{
    QString delimiterPattern("\n");
    QStringList comdatasplit = comdata.split(delimiterPattern);
    int rawtime[10];
    int i=0;
    for (i=0; i<10; i++)
        {
        if(i<=comdatasplit.size()){
        bool ok;
        rawtime[i]=comdatasplit[i].mid(16.4).toInt(&ok,10);
        setstationtime(i,rawtime[i]);
        }
        else {setstationtime(i,0);}
        }
}

void ips::setstationtime (int nr, int rawtime)
{
    int i=0;
    for (i=0; i<10; i++)
    {stationtime[nr][i+1]=stationtime[nr][i];
    stationtime[nr][0]=rawtime;
    }

}
