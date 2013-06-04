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

    //deafult
    ui->s1x->setValue(1000);ui->s1y->setValue(1000);ui->s1z->setValue(0);
    ui->s2x->setValue(2000);ui->s2y->setValue(1000);ui->s2z->setValue(0);
    ui->s3x->setValue(2000);ui->s3y->setValue(0000);ui->s3z->setValue(1000);
    ui->s4x->setValue(1000);ui->s4y->setValue(0000);ui->s4z->setValue(0);
    ui->s5x->setValue(0000);ui->s5y->setValue(0000);ui->s5z->setValue(1000);
    ui->s6x->setValue(0000);ui->s6y->setValue(1000);ui->s6z->setValue(0);
    ui->s7x->setValue(0000);ui->s7y->setValue(2000);ui->s7z->setValue(1000);
    ui->s8x->setValue(1000);ui->s8y->setValue(2000);ui->s8z->setValue(0);
    ui->s9x->setValue(2000);ui->s9y->setValue(2000);ui->s9z->setValue(1000);
    //Ende default;

    setPosStation();
    setHindernisse();

    map = new QGraphicsScene(this);
    ui->graphicsView->setScene(map);
    ui->graphicsView->scale(0.1,0.1);  //zoomen der Karte so ist platz für 7,5 meter
    //ui->graphicsView->setSceneRect(0, 0, 1000, 1000);
    //Use ScrollHand Drag Mode to enable Panning
    //ui->graphicsView->setDragMode(ScrollHandDrag);

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

    // Karte
    connect(ui->deletkoordinaten, SIGNAL(clicked()), SLOT(deletekoordinaten()));
    connect(ui->setHindernisse, SIGNAL(clicked()), SLOT(setHindernisse()));


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
void MainWindow::XbeesendCOM(unsigned long sendCOM)
{
    if (Xbeeport->isOpen() )
    {
        QString str;
        str.setNum(sendCOM);
        qDebug() << str;
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

// Hindernisse speichern

void MainWindow::setHindernisse()
{
    y.hin[0][0]=ui->h1x->value(); //x
    y.hin[0][1]=ui->h1y->value(); //y
    y.hin[0][2]=ui->h1w->value(); //x
    y.hin[0][3]=ui->h1a->value(); //y

    y.hin[1][0]=ui->h2x->value(); //x
    y.hin[1][1]=ui->h2y->value(); //y
    y.hin[1][2]=ui->h2w->value(); //x
    y.hin[1][3]=ui->h2a->value(); //y

    y.hin[2][0]=ui->h3x->value(); //x
    y.hin[2][1]=ui->h3y->value(); //y
    y.hin[2][2]=ui->h3w->value(); //x
    y.hin[2][3]=ui->h3a->value(); //y

    y.hin[3][0]=ui->h4x->value(); //x
    y.hin[3][1]=ui->h4y->value(); //y
    y.hin[3][2]=ui->h4w->value(); //x
    y.hin[3][3]=ui->h4a->value(); //y

    y.hin[4][0]=ui->h5x->value(); //x
    y.hin[4][1]=ui->h5y->value(); //y
    y.hin[4][2]=ui->h5w->value(); //x
    y.hin[4][3]=ui->h5a->value(); //y

    y.hin[5][0]=ui->h6x->value(); //x
    y.hin[5][1]=ui->h6y->value(); //y
    y.hin[5][2]=ui->h6w->value(); //x
    y.hin[5][3]=ui->h6a->value(); //y

    y.hin[6][0]=ui->h7x->value(); //x
    y.hin[6][1]=ui->h7y->value(); //y
    y.hin[6][2]=ui->h7w->value(); //x
    y.hin[6][3]=ui->h7a->value(); //y

    y.hin[7][0]=ui->h8x->value(); //x
    y.hin[7][1]=ui->h8y->value(); //y
    y.hin[7][2]=ui->h8w->value(); //x
    y.hin[7][3]=ui->h8a->value(); //y

    y.hin[8][0]=ui->h9x->value(); //x
    y.hin[8][1]=ui->h9y->value(); //y
    y.hin[8][2]=ui->h9w->value(); //x
    y.hin[8][3]=ui->h9a->value(); //y

    y.hin[9][0]=ui->h10x->value(); //x
    y.hin[9][1]=ui->h10y->value(); //y
    y.hin[9][2]=ui->h10w->value(); //x
    y.hin[9][3]=ui->h10a->value(); //y

    y.hin[10][0]=ui->h11x->value(); //x
    y.hin[10][1]=ui->h11y->value(); //y
    y.hin[10][2]=ui->h11w->value(); //x
    y.hin[10][3]=ui->h11a->value(); //y

    y.hin[11][0]=ui->h12x->value(); //x
    y.hin[11][1]=ui->h12y->value(); //y
    y.hin[11][2]=ui->h12w->value(); //x
    y.hin[11][3]=ui->h12a->value(); //y

    y.hin[12][0]=ui->h13x->value(); //x
    y.hin[12][1]=ui->h13y->value(); //y
    y.hin[12][2]=ui->h13w->value(); //x
    y.hin[12][3]=ui->h13a->value(); //y

    y.hin[13][0]=ui->h14x->value(); //x
    y.hin[13][1]=ui->h14y->value(); //y
    y.hin[13][2]=ui->h14w->value(); //x
    y.hin[13][3]=ui->h14a->value(); //y

    y.hin[14][0]=ui->h15x->value(); //x
    y.hin[14][1]=ui->h15y->value(); //y
    y.hin[14][2]=ui->h15w->value(); //x
    y.hin[14][3]=ui->h15a->value(); //y

    y.hin[15][0]=ui->h16x->value(); //x
    y.hin[15][1]=ui->h16y->value(); //y
    y.hin[15][2]=ui->h16w->value(); //x
    y.hin[15][3]=ui->h16a->value(); //y

    //Anzahl der Hindernisse ermitteln
    int i=0;
    for(i=0; i<16; i++)
    {
        if (y.hin[i][2]!=0) //Hondernisse werden akzeptiert wenn der umlaufweg bekannt gesetzt ist
        {
            y.hinanz +=1;
        }
    }



}
// ENDE Hindernisse speichern

//array Daten übergebenen PosStation
int MainWindow::getposStation(int station, int xyz)
{
    return x.posStation[station][xyz];
}

void MainWindow::wheelEvent(QWheelEvent* event) {

    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(event->delta() > 0) {
        // Zoom in
        ui->graphicsView->scale(scaleFactor, scaleFactor);
    } else {
        // Zooming out
        ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    // Don't call superclass handler here
    // as wheel is normally used for moving scrollbars
}

void MainWindow::deletekoordinaten()
{
    x.xList.clear();
    x.yList.clear();
    x.zList.clear();
}


// Zentrale Erzeugung der Karte
void MainWindow::DrawMap()
{

    map->clear();  //löschen der gesamten Karte  --OPTIMIERUNGSPOTENTIAL
    QPen normal;
    normal.setWidth(20); // hängt vom scaling ab

    int i=0;
    if(ui->printzeppelin->isChecked())
    {
    for(i=0;i<(x.xList.size()-1);i++)
    {
        //map->addLine(x.xList.at(i),x.yList.at(i),x.xList.at(i+1),x.yList.at(i+1))->setPen(normal);  //Positionsdarstellung des Zeppelins
        map->addRect(x.xList.at(i),x.yList.at(i),20,20)->setPen(normal);  // Positionsdarstellung Zeppelin - Rechtecke
    }
    }

    for(i=0;i<9;i++)
    {
        map->addRect(x.posStation[i][0],x.posStation[i][1],100,100)->setPen(normal);  // Zeichnen der Stationen
    }

    //map->addLine(x.posStation[0][0],x.posStation[0][1],x.posStation[1][0],x.posStation[1][1])->setPen(normal);  // Test Strich zwischen Station 0 und 1


}

// STOP Zentrale erzeugung der Karte






//TestButton
void MainWindow::onTestButtonClicked()
{
    unsigned long Test = 1199088177;//10199908881777;
    //Test=y.abwurf*1000000000+y.schub[0]*1000000+y.schub[1]*1000+y.schub[2];
    qDebug()<< Test;
    XbeesendCOM(Test);


   QString str;
   int ergebnis;
   int i=0;
   for(i=0;i<9;i++)
   {str.append(QString("%1").arg(x.gettime(1,i)));}
   str.append(QString("%1").arg(x.gettimef(1)));

   str.append(QString("%1").arg(x.posStation[0][0]));
   str.append(QString("%1").arg(getposStation(0,0)));
   ergebnis = x.wrapper();
   str.append(QString("%1").arg(ergebnis));

   //qDebug() << x.xList;
   y.berechneWeg();

   IPSwriteComText(str);
}
//TestButton STOP



//Funktionen GUI STOP


// refreshfunktion wird durch timer ausgelöst

void MainWindow::refresh()
{
    qDebug()<<"Koordinaterefresh";

//    x.wrapper();
//    y.xList.prepend(x.xList.at(0));
//    y.yList.prepend(x.yList.at(0));

    //y.berechneWeg();

    DrawMap();

}

// STOP refreshaktion






MainWindow::~MainWindow()
{

    delete ui;
    delete Xbeeport;
    delete IPSport;
}
