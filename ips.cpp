#include "ips.h"
#include "mainwindow.h"
#include <iostream>
#include "QDebug"


ips::ips()
{
}

void ips::setup()
{
    //Konstruktor
    for(int i=0; i<10;i++)
    {
        for(int j=0;j<15;j++)
        {
            stationtime[i][j]=0;
        }
    }
    q=0;
    w=0;
}

//---------------------------------------------------------------------------------------------------------------------
// Kommunikation mit dem IPS und Aufarbeitung der Daten (Es können nur maximal 9 Stationen berücksichstigt werden)

//.............................................................................
//Extrahiert die Laufzeit aller Stationen aus dem String vom seriellen Port (comdata)

//Funkprotokoll:
// Typ  |Stationsnummer | Zeit  |Endezeichen
// t    |01             | 1000  | \n

void ips::setdata(QString comdata)
{    
    for (int j=0; j<10; j++)        //
    {                               // Setzt das Zwischenspeicher-Array=0
        zwischenspeicher[j]=0;      //
    }                               //

    QString delimiterPattern("\n");                             //Teilt den comdata-String in die einzelnen Datenpakete, anhand der Endmarke \n
    QStringList comdatasplit = comdata.split(delimiterPattern); //

    for (int i=0; i<comdatasplit.size(); i++) //Die Schleife geht alle Datenpakete7Stücke durch
    {
        if(comdatasplit[i].length()>0){ //Verhindert Abstürze bei leeren Paketen
            if (comdatasplit[i].left(1).contains("t", Qt::CaseInsensitive)) //Fragt ab ob es ein t Paket ist, andere werden verworfen
            {if(comdatasplit[i].length()==13)   //Verhindert Abstürze bei unvollständigen Datenpaketen
                {
                    bool ok;
                    int s=0;
                    s=comdatasplit[i].mid(1,2).toInt(&ok,10)-1; //extrahiert den String mit der Stationsnummer und konvertiert in einene int (Basis 10)
                    int t=0;
                    comdatasplit[i].mid(3,10).toInt(&ok,10); //extrahiert den String mit der Zeit und konvertiert in eine int (Basis 10)
                    if(ok){
                        t=comdatasplit[i].mid(3,10).toInt(&ok,10);
                        zwischenspeicher[s]=t;
                    }
                      //Speichert die empfangenenen Werte zwischen
                    //setstationtime(s,t); //speichert die Stationsnummer und Zeit
                }
//                else{ //braucht es das? ich glaub nicht
//                    if(comdatasplit[i].length()>3)
//                    {
//                        bool ok;
//                        int s=0;
//                        comdatasplit[i].mid(1,2).toInt(&ok,10);
//                        if(ok)
//                        {
//                            s=comdatasplit[i].mid(1,2).toInt(&ok,10)-1;
//                        }
//                        else
//                        {
//                            //                    s=comdatasplit[i-1].mid(1,2).toInt(&ok,10);  //geht nicht
//                        }
//                        //setstationtime(s,0);
//                        keepstationtime(s);
//                    }
//                    else
//                    {
//                        //                    bool ok;
//                        int s=0;
//                        //                    s=comdatasplit[i-1].mid(1,2).toInt(&ok,10); //geht nicht
//                        //setstationtime(s,0);
//                        keepstationtime(s);
//                    }
//              }
//
            }
        }
    }
    for(int k=0; k<10; k++)
    {
        setstationtime(k, zwischenspeicher[k]);
        //qDebug()<<zwischenspeicher[k];
    }
}

//ENDE Extrahiert die Laufzeit aller Stationen
//.............................................................................

//.............................................................................
//Speichert den aktuellen Wert der Stationen an der Stelle [0] im Array und verschiebt alle alten Werte nach unten
void ips::setstationtime (int nr, int rawtime)
{
    int i=0;
    for (i=14; i>0; i--)
    {stationtime[nr][i]=stationtime[nr][i-1];}
    stationtime[nr][0]=rawtime;
    //qDebug()<<stationtime[nr][0];
}
//void ips::keepstationtime (int nr)
//{
//    int i=0;
//    for (i=8; i>0; i--)
//    {stationtime[nr][i]=stationtime[nr][i-1];}
//    stationtime[nr][0]=stationtime[nr][1];


//}
// ENDE Stationen speichern
//.............................................................................

//Einfache Funktion die eine Zeit direkt auslesen kann (vorsicht nur die letzten 15 Werte stehen zur Verfügung, aktuellster ist 0)
int ips::gettime (int station, int time)
{
    return stationtime[station][time];
}

//.............................................................................
// Hochleistungfilter ;) Neu Neu Neu

// Dieser Filter nimmt die letzen Werte der Station, sortiert sie, streicht Nullwerte und ggf.
// den hohe und tiefe Werte (in der GUI einzustellen) und bildet dann das Mittel der verbleibenden

int ips::gettimef(int station)
{
    double timef=0; //Setzt den Filter am Anfang zurück
    int array[15];//Array zum Zwischenspeichern der Werte

    for (int i=0;i<15;i++)
    {
        array[i]=30000;
    }
    int filterzus=filterAnzahlMittel;
    int filterstreichenunten=filterUnten;
    int filterstreichenoben=filterOben;

    for(int i=0;i<filterzus;i++)
    {
        if(stationtime[station][i]>0 && stationtime[station][i]<30000)
        {
            array[i] = stationtime[station][i];
            qDebug()<<"Array"<<array[i];
        }
        else
        {
            if(filterzus==14||filterzus==maxFilterwerweiterung)   // damit der Filter nicht überläuft, gerade am Anfang wenn noch keine Werte da sind
            {
                qDebug()<< "Kein Wert";
                array[i] = 0;
            }
            else                                                //0 erhöhen die Zusammenfassungsanzahl um 1 und den Wert der unten getrichen wird. ergo die null wird am schluss gestrichen
            {                                                   //
                array[i] = 0;                                   //
                filterzus=filterzus+1;                          //
                filterstreichenunten=filterstreichenunten+1;    //
            }
        }
    }

    std::sort(std::begin(array), std::end(array));                          // Das array wird nach größe sortiert

    if(filterzus-filterstreichenunten<=filterstreichenoben)
    {
        filterstreichenoben=0;
        qDebug()<< "Filter: streichen oben auf Null gesetzt, wegen zu vielen Nullzeiten";
    }
    for(int i=filterstreichenunten;i<(filterzus-filterstreichenoben);i++)   //Die Höchsten und niedrigsten Werte werden gestrichen, der Rest zusammengerechnet
    {
        timef=timef+array[i];
    }

    timef=timef/(filterzus-(filterstreichenoben+filterstreichenunten)); //Mittelwert bilden

    qDebug()<< "timef: " << timef << "zusammengefasst:"<< filterzus << "unten gestrichen:" << filterstreichenunten << "oben gestrichen:" << filterstreichenoben;

    return timef;
    //return stationtime[station][0];

}

//ENDE Extrahieren und Filtern
//---------------------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------------------
//Beginn Triang.
//Der Großteil des Codes ist vom Tutor Markus erstellt und wurde hier übernommen.
//Einige Änderungen wurden durchgeführt, um es c++-konform zu machen
//Die Berechnung basiert auf dem numerischen Gradientenverfahren

//Beginn wrapper (Sammeln der Daten)
int ips::wrapper()
{

    int numstations =9;
    int i = 0;
    int j = 0;

    for (i=0;i<numstations;i++)
    {
        if ((posStation[i][3]==1) && (gettimef(i)!=0))
        {
            qDebug()<<"Nummer i"<<i;

            base_x[j] = posStation[i][0];       //Die Koordinaten der
            base_y[j] = posStation[i][1];       //Bodenstationen werden
            base_z[j] = posStation[i][2];       //abgepeichert

            //Berechnung des Abstandes des Zeppelins zu den einzelnen Bodenstationen anhand der Laufzeiten
            r[j] = gettimef(i)*0.343;
            j++;
        }
    }
    qDebug()<<"Anzahl"<<j;
    n = j;
    if (xList.size()!=0){
        start_x = xList.at(0);
        start_y = yList.at(0);
        start_z = 5000;//zList.at(0);
    }
    else {
        start_x = 0;
        start_y = 0;
        start_z = 5000;
    }

    rechne();
    pos_filter();
    qDebug()<<"Position z"<<posz;
    qDebug() << "Posx: " << xList.at(0) << "Posy: " << yList.at(0) <<"Posz: "<< zList.at(0);

    int genauigkeit = BetragVektor(posx, posy, xList.at(0), yList.at(0));
    return genauigkeit;
}
//Ende wrapper

//.............................................................................

//Positionen filtern
void ips::pos_filter()
{
    if ((xList.size()< 6) || (BetragVektor(posx, posy, xList.at(0), yList.at(0)) < max_abw_flug)||(q>2))
    {
        xList.prepend(posx);
        yList.prepend(posy);
        q=0;
    }
    else
    {
        q++;
        qDebug()<<"Fehler - q:"<<q;
    }

    if(ultraschall==false)
    {
        qDebug()<<"Position z"<<posz;
        if ((zList.size()< 6) || (abs(zList.at(0)- posz) < max_abw_hoehe) || (w>2))
        {
            zList.prepend(posz);
            w=0;
        }
        else
        {
            w++;
            qDebug()<<"Fehler - w:"<<w;
        }
    }
}
//Ende Position filtern

//.............................................................................

//Abstand zweier Vektoren bzw Betrag eines Vektors............................
int ips::BetragVektor(int x_1, int y_1, int x_2, int y_2)
{
    //Berechnet den Abstand zweier Punkte
    return sqrt(((x_2-x_1)*(x_2-x_1))+((y_2-y_1)*(y_2-y_1)));
}
//Ende Abstandsbrechnung......................................................

//.............................................................................

//Start Berechnung der Koordinaten
void ips::rechne()
{
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
        gradf(x,y,z);
        richtung[0] = erg[0];
        richtung[1] = erg[1];
        richtung[2] = erg[2];
        //Schrittweite nach Armijo (google it!!)
        schritt = armijo(x,y,z);
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
    qDebug() << "Anzahl der Schleifen:" << i;

    if(i==300)
    {
        posx=0;
        posy=0;
        posz=0;
        qDebug()<< "Keine Annäherung in 300 Schritten möglich";
    }
    else
    {
        posx = x_neu;
        posy = y_neu;
        posz = z_neu;
    }
}
//Ende Berechnung der Koordinaten
//.............................................................................

//.............................................................................
//Start zu minimierende Funktion f
double ips::f(double x, double y, double z)
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
//.............................................................................

//.............................................................................
//Start Berechnung des Gradienten f (gradf)

void ips::gradf(double x, double y, double z)
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
//.............................................................................

//.............................................................................
//Start Bestimmung der armijo-Schrittweite(armijo)

double ips::armijo(double x, double y, double z)
{
    double schritt = 1;
    int i = 0;
    double ls = 0.0;
    double rs = 0.0;
    //double *gradient;
    //gradf(x,y,z,n);
    double gradient[3];
    gradient[0] = erg[0];
    gradient[1] = erg[1];
    gradient[2] = erg[2];


    while (i < 1200)
    {
        ls = f(x+schritt*(-gradient[0]),y+schritt*(-gradient[1]),z+schritt*(-gradient[2]));
        rs = f(x,y,z) + 0.5*schritt*(-gradient[0]*gradient[0]-gradient[1]*gradient[1]-gradient[2]*gradient[2]);
        if (ls <= rs)
            break;
        schritt = schritt / 2;
        i++;
    }

    return schritt;
}
//Ende armijo
//.............................................................................

//Ende Triang.
//---------------------------------------------------------------------------------------------------------------------
