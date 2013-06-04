#ifndef WEG_H
#define WEG_H

#include <QList>

class weg
{
public:

//Variablen:

    int hin[16][4];     //Erste Spalte: x-Koordinate, 2.: y-Koordinate 3.: rechts oder links rum 4.: Abstand Hindernis/Wand
    int hinanz;       // Anzahl der eingetragenen Hindernisse
    int schub[3];    //Schub link, schub rechts, schub höhe
    int spannweite;
    int abwurf; //0 ist nein 1 ist ja
    int strecke; // welche Stange wird angeflogen

    int EP[2]; //Eintrittspunkt in Kreis
    int AP[2]; //Ausrittspunkt aus Kreis
    //Zeppelindaten

    QList<int> Ausrichtung;  //Winkel in Grad gegen 0

    QList<int> xList;
    QList<int> yList;
    QList<int> zList;

    bool modus; //0 ist normal, 1 ist Notfall

    //Notfallplan variable
    int notfallziel[3]; //x und y Koordinate Notfallplan Ziel + Ausrichtungswinkel
    int zieltol; // Tolleranz ob Ziel erreicht ist

    //Kreisradien um die Stangen
    int kreisradius[16];
    int naechsterNachbar[16];

    //gerade(0) oder kreis(1)
    int flug;

//Funktionen:

    //Wegberechnung
    void moduscheck();
    void berechneWeg();
    void berechneRadien();
    double Winkel(double x, double y, int i);
    double Abs(double x);
    void GetCollisionPoint(double P_x, double P_y, double Q_x, double Q_y, double r1, double r2, double *res1, double *res2, double *res3, double *res4);
    void Tangentenberechnung(double mittelx, double mittely, double rad);
    int Abweichung(int Radius, int Hindernisnummer); //Abweichung vom Radius um das Hindernis

    //Notfallplan
    void notfallplan();
    void notfallplananfang(int AnfangsSollwinkel);
    void notfallplanende(int EndSollwinkel);

    //Motorsteuerung
    void geradeaus(int geschwindigkeit);  //Geschw. von 0 bis 100
    void kurve(bool linksrechts, int radius);
    void standdrehung(int winkel);

    //Test
    void SetPoints(int p)
    {
         Points = p;
    }

    int GetPoints()
    {
         return Points;
    }
    int Points;
    //Test Ende
};


#endif // WEG_H
