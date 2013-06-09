#ifndef WEG_H
#define WEG_H

#include <QList>
#include "define.h"

class weg
{
public:

//Variablen:


    //Variablen aus der GUI

    int abweichungGUI[3];
    int spannweite;
    int hin[16][4];     // Erste Spalte: x-Koordinate, 2.: y-Koordinate 3.: rechts oder links rum 4.: Abstand Hindernis/Wand
    int hinanz;         // Anzahl der eingetragenen Hindernisse
    int abwurfkoordinate[2];      // Abwurfkoordinaten
    int zielkoordinaten[2]; // Zielkoordinaten


    //Variablen für die Berechnung

    int strecke; // welche Stange wird angeflogen
    bool modus;
    int EP[2]; //Eintrittspunkt in Kreis
    int AP[2]; //Ausrittspunkt aus Kreis


    //Zeppelindaten

    QList<int> Ausrichtung;  //Winkel in Grad gegen 0
    QList<int> xList;
    QList<int> yList;
    QList<int> zList;

    int schub[3];    //Schub link, schub rechts, schub höhe
    int abwurfmodus; //0 ist nein 1 ist ja


    //Notfallplan variable
    int notfallziel[3]; //x und y Koordinate Notfallplan Ziel + Ausrichtungswinkel
    int zieltol; // Tolleranz ob Ziel erreicht ist


    //Kreisradien um die Stangen
    int kreisradius[16];
    int naechsterNachbar[16];


    //gerade(0) oder kreis(1)
    //int flug;

//Funktionen:

    void start();
    int GetAbweichung(); //IST/Sollwertabweichung in %
    void punktabweichung();

    //Wegberechnung
    void moduscheck();
    void berechneWeg();
    void berechneRadien();
    double GetWinkel(double x, double y, int i);
    double Abs(double x);
    void GetCollisionPoint(double P_x, double P_y, double Q_x, double Q_y, double r1, double r2, double *res1, double *res2, double *res3, double *res4);
    void Tangentenberechnung(double mittelx, double mittely, double Bezugspunkt_x, double Bezugspunkt_y, double rad);
    //int Abweichung(int Radius, int Hindernisnummer); //Abweichung vom Radius um das Hindernis

    //Allgemeine Funktionen
    int punktabweichung(int x_1, int y_1,int x_2,int y_2);

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
