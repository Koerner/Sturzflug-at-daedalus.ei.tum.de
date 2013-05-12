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

    int stationtime[10][10];//={{0}};


private:
    int x[50];
    int y[50];
    int z[50];
    int xf[1000];
    int yf[1000];
    int zf[1000];
    void setstationtime(int nr, int rawtime);

};

#endif // IPS_H
