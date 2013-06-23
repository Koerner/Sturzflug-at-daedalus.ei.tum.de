#ifndef IPS_H
#define IPS_H
#include <QString>
#include <QStringList>
#include <QArrayData>
#include <QList>

class ips
{
public:
    //ips();
    //int getpos();
    void setdata(QString comdata);
    int gettime (int station, int time);
    int gettimef(int station);
    int wrapper();

    int filterAnzahlMittel;     //Zahl der Werte die gemittelt werden sollen
    int filterUnten;            //Zahl der niedrigsten Werte die gestrichen werden
    int filterOben;             //Zahl der höchsten Werte die gestrichen werden

    int stationtime[10][10];//={{0}};
    int posStation[10][3];
    QList<int> xList;
    QList<int> yList;
    QList<int> zList;

private:
    int x[50];
    int y[50];
    int z[50];
    int xf[1000];
    int yf[1000];
    int zf[1000];
    double erg[3];
    double posx;
    double posy;
    double posz;
    double start_x;// = 10000;
    double start_y;// = 10000;
    double start_z;// = 10000;
    //Koord der Basen
    double base_x[10];
    double base_y[10];
    double base_z[10];
    //Abstaende zu den Basen
    double r[10];
    //Anz der Basen
    int n;// = 0;


    void setstationtime(int nr, int rawtime);
    void keepstationtime (int nr);
    void rechne();
    double f(double x, double y, double z, int n);
    void gradf(double x, double y, double z, int n);
    double armijo(double x, double y, double z, int n);

};

#endif // IPS_H
