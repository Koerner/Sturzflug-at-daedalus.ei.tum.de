#ifndef WEG_H
#define WEG_H

#include <QList>

class weg
{
public:

//Variablen:

    int hin[16][4];     //Erste Spalte: x-Koordinate, 2.: y-Koordinate 3.: rechts oder links rum 4.: Abstand Hindernis/Wand
    int hinanz;       // Anzahl der eingetragenen Hindernisse

    int S[2][2];
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

//Funktionen:

    //Wegberechnung
    void moduscheck();
    void berechneWeg();
    void berechneRadien();
    //double Abs(double x);
    //void GetCollisionPoint(Point P1, Point P2, double r1, double r2, Result *res);
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
};
class Point
{
     public:

     Point()
     {
          x = y = 0;
     }

     Point(double x_, double y_)
     {
          x = x_;
          y = y_;
     }

     double x;
     double y;
};

class Result
{
     public:

     Result()
     {
          Points = 0;
     }

     void SetPoints(int p)
     {
          Points = p;
     }

     int GetPoints()
     {
          return Points;
     }

     Point P1;
     Point P2;

     private:

     int Points;
};

#endif // WEG_H
