#include "ips.h"
#include "mainwindow.h"
#include <iostream>
#include "QDebug"


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
                    //setstationtime(s,0);
                    keepstationtime(s);
                }
                else
                {
//                    bool ok;
                    int s=0;
//                    s=comdatasplit[i-1].mid(1,2).toInt(&ok,10); //geht nicht
                    //setstationtime(s,0);
                    keepstationtime(s);
                }


            }
            }
        }
    }
}
//ENDE Extrahiert die Laufzeit aller Stationen

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

// Hochleistungfilter ;) Neu Neu Neu
int ips::gettimef(int station)
{
    int i=0;
    double timef=0;
    int array[10];
    int filterzus=1;            //Zahl der Werte die gemittelt werden sollen
    int filterstreichenunten=0; //Zahl der niedrigsten Werte die gestrichen werden
    int filterstreichenoben=0;  //Zahl der höchsten Werte die gestrichen werden

    for(i=0;i<filterzus;i++)
    {
        if(stationtime[station][i]!=0)
        {
        array[i] = stationtime[station][i];
        }
        else
        {
            if(filterzus==9)                                    // damit der Filter nicht überläuft, gerade am Anfang wenn noch keine Werte da sind
            {
                qDebug()<< "Kein Wert";
                array[i] = stationtime[station][i];
            }
            else                                                //0 erhöhen die Zusammenfassungsanzahl um 1 und den Wert der unten getrichen wird. ergo die null wird am schluss gestrichen
            {
                filterzus=filterzus+1;
                filterstreichenunten=filterstreichenunten+1;
            }
        }
    }

    std::sort(array, array+filterzus);                          // Das array wird nach größe sortiert
    timef=0;

    for(i=filterstreichenunten;i<(filterzus-filterstreichenoben);i++)   //Die Höchsten und niedrigsten Werte werden gestrichen, der Rest zusammengerechnet
            {timef=timef+array[i];}

    timef=timef/(filterzus-(filterstreichenoben+filterstreichenunten)); //Mittelwert bilden

    qDebug()<< "timef: " << timef << "zusammengefasst:"<< filterzus << "unten gestrichen:" << filterstreichenunten << "oben gestrichen:" << filterstreichenoben;

    return timef;

}
//ENDE Filter




//Beginn Triang.
//Der Großteil des Codes ist vom Tutor Markus erstellt und wurde hier übernommen.
//Einige Änderungen wurden durchgeführt, um es c++-konform zu machen
//Die Berechnung basiert auf dem numerischen Gradientenverfahren


//Beginn wrapper (Sammeln der Daten)
int ips::wrapper()
{

    int numstations =9;
    int i = 0;

    //    millis = int(round(time.time() * 1000))

    for (i=0;i<numstations;i++) {

        base_x[i] = posStation[i][0];       //Die Koordinaten der
        base_y[i] = posStation[i][1];       //Bodenstationen werden
        base_z[i] = posStation[i][2];       //abgepeichert

        //Berechnung des Abstandes des Zeppelins zu den einzelnen Bodenstationen anhand der Laufzeiten
        r[i] = gettimef(i)*0.343;
        //r[0]=1978*0.34;
        //r[1]=3675*0.34;
        //r[2]=5139*0.34;
        //r[3]=4366*0.34;
        //r[4]=4953*0.34;
        //r[5]=3649*0.34;
        //r[6]=3851*0.34;
        //r[7]=2789*0.34;
        //r[8]=3790*0.34;


    }
    n = numstations;
    if (xList.size()!=0){
        start_x = xList.at(0);
        start_y = yList.at(0);
        start_z = zList.at(0);
    }
    else {
        qDebug() << "ohne startwert";
        start_x = 0;
        start_y = 0;
        start_z = 0;
    }


    qDebug() << "rechne";

    rechne();

    qDebug() << "fertig rechne";

    xList.prepend(posx);//posx
    yList.prepend(posy);//posy
    zList.prepend(posz);//posz

    qDebug() << "Posx: " << posx << "Posy: " << posy <<"Posz: "<< posz;

    int genauigkeit = xList.at(0);
    return genauigkeit;
}
//Ende wrapper


//Start Berechnung der Koordinaten
void ips::rechne()
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
//qDebug() << schritt;
        x_neu = x - schritt*richtung[0];
//qDebug() << x_neu;
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
    qDebug() << "Schleife:" << i;

//    x = x_neu;
//    y = y_neu;
//    z = z_neu;

//    posx = x;
//    posy = y;
//    posz = z;
    if(i==300)
    {
        posx=0;
        posy=0;
        posz=0;
        qDebug()<< "Keinne Annäherung in 300 Schritten möglich";
    }
    else
    {
        posx = x_neu;
        posy = y_neu;
        posz = z_neu;
    }



    //int ret = 0;
    //return ret;
}
//Ende Berechnung der Koordinaten


//Start zu minimierende Funktion f
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
//Ende zu minimierende Funktion f


//Start Berechnung des Gradienten f (gradf)
void ips::gradf(double x, double y, double z, int n)
{

    erg[0] = 0.0; erg[1] = 0.0; erg[2] = 0.0;
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

}
//Ende gradf


//Start Bestimmung der armijo-Schrittweite(armijo)
double ips::armijo(double x, double y, double z, int n)
{
    double schritt = 1;
    int i = 0;
    double ls = 0.0;
    double rs = 0.0;
    //double *gradient; Bärle
    //gradf(x,y,z,n);
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
//Ende armijo

//Ende Triang.
