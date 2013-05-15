#ifndef IPS_H
#define IPS_H
#include <QString>
#include <QStringList>
#include <QArrayData>

class ips
{
public:
    //ips();
    //int getpos();
    void setdata(QString comdata);
    int gettime (int station, int time);
    double gettimef(int station);
    int aufruf_wrapper();

    int stationtime[10][10];//={{0}};
    double pos_x[10];
    double pos_y[10];
    double pos_z[10];

    int posStation[10][3];

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


    void setstationtime(int nr, int rawtime);
    int rechne();
    double f(double x, double y, double z, int n);
    void gradf(double x, double y, double z, int n);
    double armijo(double x, double y, double z, int n);
    double wrapper(double start[3], double radius[], int nn, double *refx, double *refy, double *refz);

};

#endif // IPS_H
