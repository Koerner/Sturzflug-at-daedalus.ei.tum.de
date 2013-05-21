#ifndef WEG_H
#define WEG_H

#include <QList>

class weg
{
public:
    int hin_x[16][2];
    int hin_y[16][2];

    QList<int> xList;
    QList<int> yList;
    QList<int> zList;

    void berechneWeg();
};

#endif // WEG_H
