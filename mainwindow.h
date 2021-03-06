#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QDialog>
#include <qstring>


#include "qextserialport.h"
#include "qextserialenumerator.h"
#include "ui_mainwindow.h"
#include "ips.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTextStream>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

#include "vector"
#include "QPainter"
#include "QDebug"

#include "weg.h"
#include "simulation.h"
#include "define.h"

namespace Ui {
class MainWindow;
}


class QTimer;
class QextSerialPort;
class QextSerialEnumerator;

class MainWindow : public QMainWindow
{
public:

    void XbeewriteComText(QString writeComText);
    void XbeesendCOM(unsigned long sendCOM);
    void IPSwriteComText(QString writeComText);
    void IPSsendCOM(int sendCOM);
    void schubsenden();
    void offset();
    int getposStation(int station, int xyz);
    ips x;
    weg y;
    simulation z;

    //Schub offset
    int schuboffset[3];




    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    
    void DrawMap();


private:
    Ui::MainWindow *ui;
    QTimer *Xbeetimer;
    QTimer *IPStimer;
    QTimer *Refreshtimer;
    QextSerialPort *Xbeeport;
    QextSerialPort *IPSport;
    QextSerialEnumerator *enumerator;
    QGraphicsScene *map;

    //Handsteuerung
    int geradeabweichung;
    int hoehenschubHand;
    int imu_wert;
    int imu_init;
    void keyPressEvent(QKeyEvent *qkeyevent);
    void setup();







//GUI Funktionen
private Q_SLOTS:
    void XbeeonBaudRateChanged(int idx);
    void XbeeonPortAddedOrRemoved();
    void XbeeonPortNameChanged(const QString &name);
    void XbeeonReadyRead();

    void IPSonBaudRateChanged(int idx);
    void IPSonPortAddedOrRemoved();
    void IPSonPortNameChanged(const QString &name);
    void IPSonReadyRead();

    void refresh();
    void deletekoordinaten();
    virtual void wheelEvent(QWheelEvent* event);

    void onTestButtonClicked();
    void onreset_imuClicked();

    void setPosStation();
    void setHindernisse();
    void setrefreshrate();
    void setAbweichung();
    void setZiel(); //ABwurfkoordinaten
    void setZieltolleranz();
    void setHoehe();
    void setHandsteuerung();
    void setFilter();
    void setRueckschub();
    void setUltraschall();

    //Handsteuerung Buttons
    void vor();
    void back();
    void links();
    void rechts ();
    void hoch();
    void runter();
    void stop();
    void abwurf();



};

#endif // MAINWINDOW_H
