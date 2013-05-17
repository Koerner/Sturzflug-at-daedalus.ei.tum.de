#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vector"
#include "QPainter"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setPosStation();

    map = new QGraphicsScene(this);
    ui->graphicsView->setScene(map);

    // GUI Einstellunegn

    setWindowTitle(tr("Zentrale - Sturzflug@daedalus"));

    //PortBox ComPort Einstellungen
        //COM Ports suchen
        foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        ui->XbeeportBox->addItem(info.portName);
        foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        ui->IPSportBox->addItem(info.portName);

        //selber eintargen Möglich machen
        ui->XbeeportBox->setEditable(true);
        ui->IPSportBox->setEditable(true);
    //Port Box STOP

    //BaudRateBox kompatible Werte mit Win und Linux
    ui->XbeeBaudRateBox->addItem("1200", BAUD1200);
    ui->XbeeBaudRateBox->addItem("2400", BAUD2400);
    ui->XbeeBaudRateBox->addItem("4800", BAUD4800);
    ui->XbeeBaudRateBox->addItem("9600", BAUD9600);
    ui->XbeeBaudRateBox->addItem("19200", BAUD19200);
    ui->XbeeBaudRateBox->addItem("115200", BAUD115200);
    ui->XbeeBaudRateBox->setCurrentIndex(3); //Vorsicht nur Ändern wenn auch PorSettings geändert
    ui->IPSBaudRateBox->addItem("115200", BAUD115200); //IPS nur mit dieser BaudRate
    ui->IPSBaudRateBox->setCurrentIndex(0); //Vorsicht nur Ändern wenn auch PorSettings geändert
    // BaudRateBox STOP

    //ComText Box
    ui->XbeeComText->setReadOnly(true);
    ui->IPSComText->setReadOnly(true);
    //ComText Box STOP

    Xbeetimer = new QTimer(this);
    Xbeetimer->setInterval(40);
    IPStimer = new QTimer(this);
    IPStimer->setInterval(40); //könnte probleme lösen

    Filtertimer = new QTimer(this);
    Filtertimer->setInterval(1000); //Aktuallisierungsrate 1sec

    //Vordefinierte Einstelluneg
    PortSettings Xbeesettings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    Xbeeport = new QextSerialPort(ui->XbeeportBox->currentText(), Xbeesettings, QextSerialPort::Polling);
    PortSettings IPSsettings = {BAUD115200, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    IPSport = new QextSerialPort(ui->IPSportBox->currentText(), IPSsettings, QextSerialPort::Polling);
    //Vordefinierte Einstellungen STOP

    //enumerator = new QextSerialEnumerator(this);
    //enumerator->setUpNotifications();

    //Connectoren

    //globale Connectoren
    connect(ui->TestButton, SIGNAL(clicked()), SLOT(onTestButtonClicked()));


    //Xbee connectoren
    connect(ui->XbeeBaudRateBox, SIGNAL(currentIndexChanged(int)), SLOT(XbeeonBaudRateChanged(int)));
    connect(ui->XbeeportBox, SIGNAL(editTextChanged(QString)), SLOT(XbeeonPortNameChanged(QString)));
    connect(Xbeetimer, SIGNAL(timeout()), SLOT(XbeeonReadyRead()));
    connect(Xbeeport, SIGNAL(readyRead()), SLOT(XbeeonReadyRead()));

    //IPS connectoren
    connect(ui->IPSBaudRateBox, SIGNAL(currentIndexChanged(int)), SLOT(IPSonBaudRateChanged(int)));
    connect(ui->IPSportBox, SIGNAL(editTextChanged(QString)), SLOT(IPSonPortNameChanged(QString)));
    connect(IPStimer, SIGNAL(timeout()), SLOT(IPSonReadyRead()));
    connect(IPSport, SIGNAL(readyRead()), SLOT(IPSonReadyRead()));
    connect(ui->posStationSetzen, SIGNAL(clicked()), SLOT(setPosStation()));

    //refresh connector ZENTRALES Takt-Element
    connect(Filtertimer, SIGNAL(timeout()), SLOT(refresh()));
    Filtertimer->start();


    //ConnectorenSTOP




}

 //Funktionen GUI

//Baud Rate
void MainWindow::XbeeonBaudRateChanged(int idx)
{
    Xbeeport->setBaudRate((BaudRateType)ui->XbeeBaudRateBox->itemData(idx).toInt());
}
void MainWindow::IPSonBaudRateChanged(int idx)
{
    IPSport->setBaudRate((BaudRateType)ui->IPSBaudRateBox->itemData(idx).toInt());
}
//Baus Rate STOP


// Port Änderung
void MainWindow::XbeeonPortAddedOrRemoved()
{
    QString current = ui->XbeeportBox->currentText();

    ui->XbeeportBox->blockSignals(true);
    ui->XbeeportBox->clear();
    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        ui->XbeeportBox->addItem(info.portName);

    ui->XbeeportBox->setCurrentIndex(ui->XbeeportBox->findText(current));
    ui->XbeeportBox->blockSignals(false);
}
void MainWindow::IPSonPortAddedOrRemoved()
{
    QString current = ui->IPSportBox->currentText();

    ui->IPSportBox->blockSignals(true);
    ui->IPSportBox->clear();
    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        ui->IPSportBox->addItem(info.portName);

    ui->IPSportBox->setCurrentIndex(ui->IPSportBox->findText(current));
    ui->IPSportBox->blockSignals(false);
}
//Port Änderung STOP

//Senden an ComPort
void MainWindow::XbeewriteComText(QString writeComText)
{
    ui->XbeeComText->moveCursor(QTextCursor::End);
    ui->XbeeComText->insertPlainText(writeComText);
    ui->XbeeComText->moveCursor(QTextCursor::End);
}
void MainWindow::IPSwriteComText(QString writeComText)
{
    ui->IPSComText->moveCursor(QTextCursor::End);
    ui->IPSComText->insertPlainText(writeComText);
    ui->IPSComText->moveCursor(QTextCursor::End);
}
// Senden an ComPort STOP

// Ändern des ComPorts und öffnen
void MainWindow::XbeeonPortNameChanged(const QString & /*name*/)
{
    // ggf. offenen Comport schließen

    if (Xbeeport->isOpen())
    {
        Xbeeport->close();
        XbeewriteComText("---\nCom Port geschlossen\n");
    }
    else{}
    // STOP

    // Com Port setzen und öffnen
     Xbeeport->setPortName(ui->XbeeportBox->currentText());  //Com setzen
     Xbeeport->setQueryMode(QextSerialPort::Polling); //Pollin Mode einstellen
     Xbeeport->open(QIODevice::ReadWrite);

     if (Xbeeport->isOpen()) { //Abfrage ob's geklappt hat
         XbeewriteComText("---\nCom Port offen\n---\n");
        }
        else {
         XbeewriteComText("---\nFehler - konnte Com Port nicht oeffnen \n---\n");
              }
     //STOP
        //Da polling mode, Ein QTimer
        if (Xbeeport->isOpen() && Xbeeport->queryMode() == QextSerialPort::Polling)
        Xbeetimer->start();
        else
        Xbeetimer->stop();
     }
void MainWindow::IPSonPortNameChanged(const QString & /*name*/)
{
    // ggf. offenen Comport schließen

    if (IPSport->isOpen())
    {
        IPSport->close();
        IPSwriteComText("---\nCom Port geschlossen\n");
    }
    else{}
    // STOP

    // Com Port setzen und öffnen
     IPSport->setPortName(ui->IPSportBox->currentText());  //Com setzen
     IPSport->setQueryMode(QextSerialPort::Polling); //Pollin Mode einstellen
     IPSport->open(QIODevice::ReadWrite);

     if (IPSport->isOpen()) { //Abfrage ob's geklappt hat
         IPSwriteComText("---\nCom Port offen\n---\n");
        }
        else {
         IPSwriteComText("---\nFehler - konnte Com Port nicht oeffnen \n---\n");
              }
     //STOP
        //Da polling mode, Ein QTimer
        if (IPSport->isOpen() && IPSport->queryMode() == QextSerialPort::Polling)
        IPStimer->start();
        else
        IPStimer->stop();
     }
//Ädern des Com Ports und öffnen STOP



// ComPort lesen
void MainWindow::XbeeonReadyRead()
{
    if (Xbeeport->bytesAvailable()) {
        XbeewriteComText ("->");
        XbeewriteComText(QString::fromLatin1(Xbeeport->readAll()));
        XbeewriteComText ("\n");
    }
}
void MainWindow::IPSonReadyRead()
{
    if (IPSport->bytesAvailable()) {
        QString comdata = QString::fromLatin1(IPSport->readAll());
        IPSwriteComText ("->");
        IPSwriteComText(comdata);
        IPSwriteComText ("\n");

        x.setdata(comdata);
    }
}
//ComPort lesen STOP

//ComPort schreiben
void MainWindow::XbeesendCOM(int sendCOM)
{
    if (Xbeeport->isOpen() )
    {
        QString str;
        str.append(QString("%1").arg(sendCOM));
        ui->XbeesendEdit->insertPlainText(str);
        Xbeeport->write(ui->XbeesendEdit->toPlainText().toLatin1());
        // Textausgabe des gesendetetn
        XbeewriteComText("<-");
        XbeewriteComText(ui->XbeesendEdit->toPlainText().toLatin1());
        XbeewriteComText("\n");
        ui->XbeesendEdit->clear ();//loeschen sonst wird das alte immer wieder mitgesendet
    }
    else
    {
        XbeewriteComText("---\nPort nicht offen, Senden Fehlgeschlagen\n---\n");
    }

}
void MainWindow::IPSsendCOM(int sendCOM)
{
    if (IPSport->isOpen() )
    {
        QString str;
        str.append(QString("%1").arg(sendCOM));
        ui->IPSsendEdit->insertPlainText(str);
        IPSport->write(ui->IPSsendEdit->toPlainText().toLatin1());
        // Textausgabe des gesendetetn
        IPSwriteComText("<-");
        IPSwriteComText(ui->IPSsendEdit->toPlainText().toLatin1());
        IPSwriteComText("\n");
        ui->IPSsendEdit->clear ();//loeschen sonst wird das alte immer wieder mitgesendet
    }
    else
    {
        IPSwriteComText("---\nPort nicht offen, Senden Fehlgeschlagen\n---\n");
    }

}

//ComPort schreiben STOP

//Positionen der Bodenstationen speichern

void MainWindow::setPosStation()
{
    x.posStation[0][0]=ui->s1x->value(); //x
    x.posStation[0][1]=ui->s1y->value(); //y
    x.posStation[0][2]=ui->s1z->value(); //z

    x.posStation[1][0]=ui->s2x->value(); //x
    x.posStation[1][1]=ui->s2y->value(); //y
    x.posStation[1][2]=ui->s2z->value(); //z

    x.posStation[2][0]=ui->s3x->value(); //x
    x.posStation[2][1]=ui->s3y->value(); //y
    x.posStation[2][2]=ui->s3z->value(); //z

    x.posStation[3][0]=ui->s4x->value(); //x
    x.posStation[3][1]=ui->s4y->value(); //y
    x.posStation[3][2]=ui->s4z->value(); //z

    x.posStation[4][0]=ui->s5x->value(); //x
    x.posStation[4][1]=ui->s5y->value(); //y
    x.posStation[4][2]=ui->s5z->value(); //z

    x.posStation[5][0]=ui->s6x->value(); //x
    x.posStation[5][1]=ui->s6y->value(); //y
    x.posStation[5][2]=ui->s6z->value(); //z

    x.posStation[6][0]=ui->s7x->value(); //x
    x.posStation[6][1]=ui->s7y->value(); //y
    x.posStation[6][2]=ui->s7z->value(); //z

    x.posStation[7][0]=ui->s8x->value(); //x
    x.posStation[7][1]=ui->s8y->value(); //y
    x.posStation[7][2]=ui->s8z->value(); //z

    x.posStation[8][0]=ui->s9x->value(); //x
    x.posStation[8][1]=ui->s9y->value(); //y
    x.posStation[8][2]=ui->s9z->value(); //z
}

//STOP Positionen der Bodenstationen speichern

//array Daten übergebenen PosStation
int MainWindow::getposStation(int station, int xyz)
{
    return x.posStation[station][xyz];
}


// Zentrale Erzeugung der Karte
void MainWindow::DrawMap()
{
    map->clear();  //löschen der gesamten Karte  --OPTIMIERUNGSPOTENTIAL
    int i=0;
    for(i=0;i<x.xList.size();i++)
    {
        map->addLine(x.xList.at(i),x.yList.at(i),x.xList.at(i+1),x.yList.at(i+1));  //Positionsdarstellung des Zeppelins
    }

    for(i=0;i<9;i++)
    {
        map->addRect(x.posStation[i][0],x.posStation[i][1],3,3);  // Zeichnen der Stationen
    }

    map->addLine(x.posStation[0][0],x.posStation[0][1],x.posStation[1][0],x.posStation[1][1]);  // Test Strich zwischen Station 0 und 1
}

// STOP Zentrale erzeugung der Karte






//TestButton
void MainWindow::onTestButtonClicked()
{
    int Test = 101000;
   XbeesendCOM(Test);


   QString str;

   int i=0;
   for(i=0;i<9;i++)
   {str.append(QString("%1").arg(x.gettime(1,i)));}
   str.append(QString("%1").arg(x.gettimef(1)));

   str.append(QString("%1").arg(x.posStation[0][0]));
   str.append(QString("%1").arg(getposStation(0,0)));
//   x.wrapper();
//   str.append(QString("%1").arg(x.xList.at(0)));
//   str.append(QString("%1").arg(x.yList.at(0)));
//   str.append(QString("%1").arg(x.zList.at(0)));

   //qDebug() << x.xList;

   IPSwriteComText(str);
}
//TestButton STOP



//Funktionen GUI STOP


// refreshfunktion wird durch timer ausgelöst

void MainWindow::refresh()
{
    qDebug()<<"Koordinaterefresh";
    DrawMap();

}

// STOP refreshaktion






MainWindow::~MainWindow()
{

    delete ui;
    delete Xbeeport;
    delete IPSport;
}
