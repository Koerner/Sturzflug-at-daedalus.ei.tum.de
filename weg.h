#ifndef WEG_H
#define WEG_H

#include <QList>
#include "define.h"

class weg
{
public:
// Konstruktor
    weg();
//Variablen:


    //Variablen aus der GUI

    int abweichungGUI[2]; //feste bis Regelung, und % bis Notfall
    int spannweite;
    int hin[16][4];     // Erste Spalte: x-Koordinate, 2.: y-Koordinate 3.: rechts(0) oder links(1) rum 4.: Abstand Hindernis/Wand
    int hinanz;         // Anzahl der eingetragenen Hindernisse
    int abwurfkoordinate[2];      // Abwurfkoordinaten
    int zielkoordinaten[2]; // Zielkoordinaten
    int zieltol;        //Tolleranz, wann es als Ziel erreicht gilt

    //Variablen für die Berechnung

    int hinnummer; // welche Stange wird angeflogen
    bool modus; //true: geradeaus; false: Kurve;
    int EP[2]; //Eintrittspunkt in Kreis
    int AP[2]; //Ausrittspunkt aus Kreis
    int ziel_x; //Zielkoordinaten
    int ziel_y; //Zielkoordinaten


    //Zeppelindaten

    QList<int> Ausrichtung;  //Winkel in Grad gegen 0
    QList<int> xList;
    QList<int> yList;
    QList<int> zList;

    int schub[3];    //Schub link, schub rechts, schub höhe
    int abwurfmodus; //0 ist nein 1 ist ja


    //Notfallplan variable
    bool notfallmodus; //1 ist an 0 ist aus
    int notfallziel[3]; //x und y Koordinate Notfallplan Ziel + Ausrichtungswinkel
    int notfalltol; // Tolleranz ob Ziel erreicht ist
    int notfalltolwinkel;


    //Kreisradien um die Stangen
    int kreisradius[16];
    int naechsterNachbar[16];

    //Hoehensteuerung
    int sollHoehe;
    int hoehentol;


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
    void GetCollisionPoint(double P_x, double P_y, double Q_x, double Q_y, double r1, double r2, double *res1, double *res2, double *res3, double *res4);
    void Tangentenberechnung(double mittelx, double mittely, double bezugspunktx, double bezugspunkty, double rad);
    //int Abweichung(int Radius, int Hindernisnummer); //Abweichung vom Radius um das Hindernis

    //Allgemeine Funktionen
    int punktabweichung(int x_1, int y_1,int x_2,int y_2);

    //Notfallplan
    void notfallplan();
    void notfallplananfang(int winkel);
    void notfallplanende();

    //Motorsteuerung
    void geradeaus(int streckenlaenge);  //Strecke in mm
    void geradeaus(int streckenlaenge, int abweichung); //Strecke in mm, abweichung in %
    void kurve(bool linksrechts, int radius);
    void kurve(bool linksrechts, int radius, int abweichung);
    void standdrehung(int winkel);
    void stop();

    //Hoehensteuerung

    void hoehensteuerung ();

    //Test
    int Points;
    void SetPoints(int p){Points = p;}
    int GetPoints(){return Points;}

    //Test Ende

};


#endif // WEG_H
