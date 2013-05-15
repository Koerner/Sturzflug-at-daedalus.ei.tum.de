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
    void XbeesendCOM(int sendCOM);
    void IPSwriteComText(QString writeComText);
    void IPSsendCOM(int sendCOM);
    int getposStation(int station, int xyz);
    ips x;




    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    
private:
    Ui::MainWindow *ui;
    QTimer *Xbeetimer;
    QTimer *IPStimer;
    QextSerialPort *Xbeeport;
    QextSerialPort *IPSport;
    QextSerialEnumerator *enumerator;



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

    void onTestButtonClicked();
    void setPosStation();

};

#endif // MAINWINDOW_H
