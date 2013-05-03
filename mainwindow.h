#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qextserialport.h"
#include "qextserialenumerator.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QDialog>
#include <qstring>

namespace Ui {
class MainWindow;
}


class QTimer;
class QextSerialPort;
class QextSerialEnumerator;

class MainWindow : public QMainWindow
{
public:

    void writeComText(QString writeComText);
    void sendCOM(int sendCOM);


    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    
private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QextSerialPort *port;
    QextSerialEnumerator *enumerator;


//GUI Funktionen
private Q_SLOTS:
    void onBaudRateChanged(int idx);
    void onPortAddedOrRemoved();
    void onPortNameChanged(const QString &name);
    void onReadyRead();
    void onTestButtonClicked();

};

#endif // MAINWINDOW_H
