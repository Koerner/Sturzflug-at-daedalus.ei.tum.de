#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //IPS Stationen Arduino-Simulation
    /*ui->s1x->setValue(1000);ui->s1y->setValue(1000);ui->s1z->setValue(0);
    ui->s2x->setValue(2000);ui->s2y->setValue(1000);ui->s2z->setValue(0);
    ui->s3x->setValue(2000);ui->s3y->setValue(0000);ui->s3z->setValue(1000);
    ui->s4x->setValue(1000);ui->s4y->setValue(0000);ui->s4z->setValue(0);
    ui->s5x->setValue(0000);ui->s5y->setValue(0000);ui->s5z->setValue(1000);
    ui->s6x->setValue(0000);ui->s6y->setValue(1000);ui->s6z->setValue(0);
    ui->s7x->setValue(0000);ui->s7y->setValue(2000);ui->s7z->setValue(1000);
    ui->s8x->setValue(1000);ui->s8y->setValue(2000);ui->s8z->setValue(0);
    ui->s9x->setValue(2000);ui->s9y->setValue(2000);ui->s9z->setValue(1000);*/
    //Ende IPS Stationen;

    //Karte erstellen
    map = new QGraphicsScene(this);
    ui->graphicsView->setScene(map);
    ui->graphicsView->scale(0.1,0.1);  //zoomen der Karte so ist platz für 7,5 meter

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
    Xbeetimer->setInterval(80);
    IPStimer = new QTimer(this);
    IPStimer->setInterval(155); //könnte probleme lösen

    Refreshtimer = new QTimer(this);
    Refreshtimer->setInterval(ui->refreshTime->value()); //Aktuallisierungsrate aus der GUI in ms

    //Vordefinierte Einstelluneg
    PortSettings Xbeesettings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    Xbeeport = new QextSerialPort(ui->XbeeportBox->currentText(), Xbeesettings, QextSerialPort::Polling);
    PortSettings IPSsettings = {BAUD115200, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    IPSport = new QextSerialPort(ui->IPSportBox->currentText(), IPSsettings, QextSerialPort::Polling);
    //Vordefinierte Einstellungen STOP

    //Connectoren -----------------------------------------------------------------------------------------------------

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
    connect(Refreshtimer, SIGNAL(timeout()), SLOT(refresh()));
    connect(ui->setRefresh, SIGNAL(clicked()), SLOT(setrefreshrate()));

    // Karte
    connect(ui->deletkoordinaten, SIGNAL(clicked()), SLOT(deletekoordinaten()));

    //Einstellungen
    connect(ui->setAbweichung, SIGNAL(clicked()), SLOT(setAbweichung()));
    connect(ui->setZiel, SIGNAL(clicked()), SLOT(setZiel()));
    connect(ui->setZieltolleranz, SIGNAL(clicked()), SLOT(setZieltolleranz()));
    connect(ui->setHoehe, SIGNAL(clicked()), SLOT(setHoehe()));
    connect(ui->setFilter, SIGNAL(clicked()), SLOT(setFilter()));
    connect(ui->setHindernisse, SIGNAL(clicked()), SLOT(setHindernisse()));
    connect(ui->setSchub, SIGNAL(clicked()), SLOT(setRueckschub()));
    connect(ui->ultraschall, SIGNAL(stateChanged(int)), SLOT(setUltraschall()));

    //Hansteuerung

    connect(ui->Handsteuerung, SIGNAL(stateChanged(int)), SLOT(setHandsteuerung()));

    connect(ui->vor, SIGNAL(clicked()), SLOT(vor()));
    connect(ui->back, SIGNAL(clicked()), SLOT(back()));
    connect(ui->links, SIGNAL(clicked()), SLOT(links()));
    connect(ui->rechts, SIGNAL(clicked()), SLOT(rechts()));
    connect(ui->stop, SIGNAL(clicked()), SLOT(stop()));
    connect(ui->hoch, SIGNAL(clicked()), SLOT(hoch()));
    connect(ui->runter, SIGNAL(clicked()), SLOT(runter()));
    connect(ui->abwurf, SIGNAL(clicked()), SLOT(abwurf()));

    //ENDE Connectoren ------------------------------------------------------------------------------------------------

    setup();


}

void MainWindow::setup()
{
    //SETUP: setzt wichtige Werte am Anfang. Unter anderem damit die Werte in der GUI zu den Tatsächlichen übereinstimmen.

    //GUI: Es wird sozusagen auf jeden "speichern"-Button einmal gedrückt
    setFilter();
    setZiel();
    setZieltolleranz();
    setAbweichung();
    setHoehe();
    setrefreshrate();
    setPosStation();
    setHindernisse();
    setRueckschub();
    x.setup();
    setUltraschall();


    Refreshtimer->start();  //Starten den Refresher, sonst geht garnix ;)

    //Wegberechnung

    y.hinnummer = 0;
    y.modus = true;

    //Handsteuerung initialisieren

    geradeabweichung=0;
    hoehenschubHand=0;

}

// Einstellunegn für COM Ports ----------------------------------------------------------------------------------------

// Grundlage für alle Funktionen im COM-Port Block ist die qExtSerialPort Biblothek, die Doku dazu gibts im Internet auf der Entwicklerseite
// Fast alle Funktionen sind doppelt angelegt, da es ja zwei COM-Ports zur Kommunikation gibt

//Kurz zusammengafasst, wie geht die Serielle Kommunikation:
// Ein Port wird erst festgelegt und dann geöffnet (Funktion hier: ...PortNameChanged), dadurch ist erst ein schreiben und lesen möglich
//geschrieben wird mit write() (Funktion hier: ...sendCOM
//Empfängt ein Com-Port etwas, wir das Empfangene solange gespeichert, bis die Daten abgefragt werden (Funktion hier: ...onReadyRead)




//Baud Rate
void MainWindow::XbeeonBaudRateChanged(int idx)
{
    Xbeeport->setBaudRate((BaudRateType)ui->XbeeBaudRateBox->itemData(idx).toInt());
}
void MainWindow::IPSonBaudRateChanged(int idx)
{
    IPSport->setBaudRate((BaudRateType)ui->IPSBaudRateBox->itemData(idx).toInt());
}
//ENDE Baus Rate


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
//ENDE Port Änderung


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
//ENDE Senden an ComPort



// Ändern des ComPorts und öffnen ............................................
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
    Xbeeport->setQueryMode(QextSerialPort::Polling); //Polling Mode einstellen
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
    {
        qDebug() << "Polling";
        Xbeetimer->start();
    }
    else
    {
        qDebug()<< "EventDriven";
        Xbeetimer->stop();
    }
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
//ENDE Ändern des Com Ports und öffnen ........................................



// ComPort lesen
void MainWindow::XbeeonReadyRead()
{
    //qDebug() << "lesen..";
    if (Xbeeport->bytesAvailable()) {
        //qDebug()<<"Daten..";
        QString comdata = QString::fromLatin1(Xbeeport->readAll());
        XbeewriteComText ("->");    //
        XbeewriteComText(comdata);  // Grafische Ausgabe
        XbeewriteComText ("\n");    //
        qDebug()<<"umrechnung starten";
        bool *ok=0;
        int ausrichtung=0;
        int hoehe=0;
        ausrichtung=comdata.mid(3,3).toInt(ok,10); //konvertiert die Daten in eine Integer
        qDebug() << ausrichtung;
        if(ausrichtung>320)
        {
            y.Ausrichtung.prepend(ausrichtung-500);  //konvertiert die Daten in eine Integer
            qDebug() << "Ausrichtung:" << y.Ausrichtung.at(0);
        }
        hoehe=comdata.mid(0,3).toInt(ok,10);//konvertiert die Daten in eine Integer
        if(hoehe>99 && ui->ultraschall->isChecked())
        {
            if ((y.zList.size()< 6) || (abs(y.zList.at(0)- (hoehe*10-1000)) < x.max_abw_hoehe))
            {
                y.zList.prepend(hoehe*10-1000);
                x.zList.prepend(hoehe*10-1000);
            }
        qDebug() << "Hoehe:" << y.zList.at(0);
        }

    }
}
void MainWindow::IPSonReadyRead()
{
    if (IPSport->bytesAvailable()) {
        QString comdata = QString::fromLatin1(IPSport->readAll());
        IPSwriteComText ("->");     //
        IPSwriteComText(comdata);   // Grafische Ausgabe
        IPSwriteComText ("\n");     //

        x.setdata(comdata);
    }
}
//ComPort lesen STOP



//ComPort schreiben ...............................................
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

//ENDE ComPort schreiben ......................................................

//ENDE Com Port Einstellunegn -----------------------------------------------------------------------------------------




//GUI Funktionen, hauptsächlich zum Abspeichern von Werten ------------------------------------------------------------



//Positionen der Bodenstationen speichern .....................................
void MainWindow::setPosStation()
{
    x.posStation[0][0]=ui->s1x->value(); //x
    x.posStation[0][1]=ui->s1y->value(); //y
    x.posStation[0][2]=ui->s1z->value(); //z
    if(ui->stat1_checkBox->isChecked())
    {x.posStation[0][3]=1;}
    else
    {x.posStation[0][3]=0;}

    x.posStation[1][0]=ui->s2x->value(); //x
    x.posStation[1][1]=ui->s2y->value(); //y
    x.posStation[1][2]=ui->s2z->value(); //z
    if(ui->stat2_checkBox->isChecked())
    {x.posStation[1][3]=1;}
    else
    {x.posStation[1][3]=0;}

    x.posStation[2][0]=ui->s3x->value(); //x
    x.posStation[2][1]=ui->s3y->value(); //y
    x.posStation[2][2]=ui->s3z->value(); //z
    if(ui->stat3_checkBox->isChecked())
    {x.posStation[2][3]=1;}
    else
    {x.posStation[2][3]=0;}

    x.posStation[3][0]=ui->s4x->value(); //x
    x.posStation[3][1]=ui->s4y->value(); //y
    x.posStation[3][2]=ui->s4z->value(); //z
    if(ui->stat4_checkBox->isChecked())
    {x.posStation[3][3]=1;}
    else
    {x.posStation[3][3]=0;}

    x.posStation[4][0]=ui->s5x->value(); //x
    x.posStation[4][1]=ui->s5y->value(); //y
    x.posStation[4][2]=ui->s5z->value(); //z
    if(ui->stat5_checkBox->isChecked())
    {x.posStation[4][3]=1;}
    else
    {x.posStation[4][3]=0;}

    x.posStation[5][0]=ui->s6x->value(); //x
    x.posStation[5][1]=ui->s6y->value(); //y
    x.posStation[5][2]=ui->s6z->value(); //z
    if(ui->stat6_checkBox->isChecked())
    {x.posStation[5][3]=1;}
    else
    {x.posStation[5][3]=0;}

    x.posStation[6][0]=ui->s7x->value(); //x
    x.posStation[6][1]=ui->s7y->value(); //y
    x.posStation[6][2]=ui->s7z->value(); //z
    if(ui->stat7_checkBox->isChecked())
    {x.posStation[6][3]=1;}
    else
    {x.posStation[6][3]=0;}

    x.posStation[7][0]=ui->s8x->value(); //x
    x.posStation[7][1]=ui->s8y->value(); //y
    x.posStation[7][2]=ui->s8z->value(); //z
    if(ui->stat8_checkBox->isChecked())
    {x.posStation[7][3]=1;}
    else
    {x.posStation[7][3]=0;}

    x.posStation[8][0]=ui->s9x->value(); //x
    x.posStation[8][1]=ui->s9y->value(); //y
    x.posStation[8][2]=ui->s9z->value(); //z
    if(ui->stat9_checkBox->isChecked())
    {x.posStation[8][3]=1;}
    else
    {x.posStation[8][3]=0;}
}
//STOP Positionen der Bodenstationen speichern ................................


// Hindernisse speichern ......................................................

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
    y.hinanz=0;
    for(i=0; i<16; i++)
    {
        if (y.hin[i][3]!=0) //Hondernisse werden akzeptiert wenn der umlaufweg bekannt gesetzt ist
        {
            y.hinanz +=1;
        }
    }
    qDebug() << "Anzahl der Hindernisse: " << y.hinanz;
    y.berechneRadien();//alle Kurvenradien berechnen, folgt aus Abstand zum nächsten Hindernis bzw. Wand

}
// ENDE Hindernisse speichern .................................................

void MainWindow::setrefreshrate()
{

    Refreshtimer->setInterval(ui->refreshTime->value());
    qDebug() << "Refreshrate geändert:" << ui->refreshTime->value();

    //Damit der Abweichungsfilter mit angepasst wird!
    setFilter();
}

void MainWindow::setAbweichung()
{
    y.abweichungGUI[0]=ui->AbweichungOkay->value();
    y.abweichungGUI[1]=ui->AbweichungOkayRegler->value();
    qDebug() << "alle Abweichungen gesetzt";
}

void MainWindow::setZiel()
{
    y.zielkoordinaten[0]=ui->zielkkordinaten_x->value();
    y.zielkoordinaten[1]=ui->Zielkoordinaten_y->value();
    y.abwurfkoordinate[0]=ui->abwurfpunkt_x->value();
    y.abwurfkoordinate[1]=ui->abwurfpunkt_y->value();
    y.spannweite=ui->spannweite->value();
    z.spannweite=y.spannweite;
    qDebug() << "Zielkoordinaten und Abwurfunkt gestezt:" << y.zielkoordinaten[0] << y.zielkoordinaten[1] << y.abwurfkoordinate[0] << y.abwurfkoordinate[1];
}

void MainWindow::setZieltolleranz()
{
    y.zieltol=ui->zieltolleranz->value();
    qDebug() << "Zieltolleranz gesetzt: "<< y.zieltol;
}

void MainWindow::setHoehe()
{
    y.sollHoehe=ui->SollHoehe->value();
    y.hoehentol=ui->HoeheTol->value();
    y.hoehe_d=ui->hoehe_d->value();
    qDebug()<<"Hoehe + Hoehentol gesetzt: " << y.sollHoehe << y.hoehentol;
}

void MainWindow::setHandsteuerung()
{
    qDebug() << "Handsteuerung Aenderung";
    if(ui->Handsteuerung->isChecked())
    {
        QWidget::grabKeyboard();
    }
    else
    {
        QWidget::releaseKeyboard();
    }
}

void MainWindow::setFilter()
{
    //ZEITEN Filter
    x.filterOben=ui->filteroben->value();
    x.filterUnten=ui->filterunten->value();
    x.filterAnzahlMittel=ui->filtergemittelt->value();
    x.maxFilterwerweiterung=ui->maxFiltererweiterung->value();

    //Logik Maximalabweichungsfilter
    x.max_abw_flug=(ui->max_geschw_flug->value()/10*(ui->refreshTime->value()/50));
    x.max_abw_hoehe=(ui->max_geschw_hoehe->value()/10*(ui->refreshTime->value()/50));
}

void MainWindow::setRueckschub()
{
    y.rueckschub=ui->rueckschub->value();
    y.anz_rueckschub=ui->rueckAnz->value();
}

void MainWindow::setUltraschall()
{
    if(ui->ultraschall->isChecked())
    {
        x.ultraschall=true;
        qDebug()<<"Ultraschall an";
    }
    if(!ui->ultraschall->isChecked())
    {
        x.ultraschall=false;
        qDebug()<<"Ultraschall aus";
    }
}

//ENDE Abspeichern von Werten -----------------------------------------------------------------------------------------


int MainWindow::getposStation(int station, int xyz)
{
    return x.posStation[station][xyz];
}

//Funktion die das Zoomen der Karte ermöglicht ................................
void MainWindow::wheelEvent(QWheelEvent* event) {

    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.15;
    if(event->delta() > 0) {
        // Zoom rein
        ui->graphicsView->scale(scaleFactor, scaleFactor);
    } else {
        // Zoom raus
        ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}
//Ende Kartenzoom .............................................................

void MainWindow::deletekoordinaten()
{
    x.xList.clear();
    x.yList.clear();
    x.zList.clear();
    y.xList.clear();
    y.yList.clear();
    y.zList.clear();
    z.xList.clear();
    z.yList.clear();

    y.hinnummer = 0;
    //y.hinanz = 0;
    y.modus = 1;
    y.abwurfmodus = 0;
    y.schub[0]=0;
    y.schub[1]=0;
    y.schub[2]=0;
    y.notfallmodus=0;
    y.rueck_countdown=0;
    x.wrapper();
    y.xList.prepend(x.xList.at(0));
    y.yList.prepend(x.yList.at(0));
    y.berechneWeg();

}

// Zeichnet die Karte -------------------------------------------------------------------------------------------------
void MainWindow::DrawMap()
{

    map->clear();  //löschen der gesamten Karte  --OPTIMIERUNGSPOTENTIAL
    QPen normal;
    normal.setWidth(20); // hängt vom scaling ab
    QPen penhindernisse;
    penhindernisse.setWidth(20);
    penhindernisse.setColor(Qt::red);
    QPen penziel;
    penziel.setWidth(20);
    penziel.setColor(Qt::green);
    QPen penTP;
    penTP.setWidth(20);
    penTP.setColor(Qt::blue);
    QPen penAB;
    penAB.setWidth(20);
    penAB.setColor(Qt::yellow);
    QPen penZeppelin;
    penZeppelin.setWidth(20);
    penZeppelin.setColor(Qt::gray);

    int i=0;
    if(ui->printzeppelin->isChecked())
    {
    for(i=0;i<(x.xList.size()-1);i++)
    {
        map->addRect(x.xList.at(i)-10,x.yList.at(i)-10,20,20)->setPen(normal);  // Positionsdarstellung Zeppelin - Rechtecke
    }
    }

    for(i=0;i<9;i++)
    {
        map->addRect(x.posStation[i][0]-50,x.posStation[i][1]-50,100,100)->setPen(normal);  // Zeichnen der Stationen
    }
    for(i=0;i<y.hinanz;i++)
    {

        map->addRect(y.hin[i][0]-25,y.hin[i][1]-25,50,50)->setPen(penhindernisse);  // Zeichnen der Hindernisse
    }
    map->addEllipse(y.zielkoordinaten[0]-y.zieltol,y.zielkoordinaten[1]-y.zieltol,y.zieltol*2,y.zieltol*2)->setPen(penziel);  //Zeichnet Zielkreis
    map->addEllipse(y.ziel_x-y.zieltol,y.ziel_y-y.zieltol,y.zieltol*2,y.zieltol*2)->setPen(penTP);
    map->addEllipse(y.abwurfkoordinate[0]-y.zieltol,y.abwurfkoordinate[1]-y.zieltol,y.zieltol*2,y.zieltol*2)->setPen(penAB);

    if (x.zList.size()!=0)
    {
        ui->hoeheDisplay->display(x.zList.at(0));
    }
    if (y.Ausrichtung.size()!=0)
    {
        double winkel=y.Ausrichtung.at(0)*PI/180;
        qDebug()<<"winkel: " << winkel;
        map->addLine(x.xList.at(0),x.yList.at(0),(x.xList.at(0)+300*cos(winkel)),(x.yList.at(0)+300*sin(winkel)),penZeppelin);

    }

}
// ENDE Zeichnen der Karte --------------------------------------------------------------------------------------------

// Technische Funktionen zur Steuerung --------------------------------------------------------------------------------

// Offset zu den Schubdaten hinzufügen.........................................
void MainWindow::offset()
{
    //qDebug() << "offset hinzufühen";
    if(!y.schub[0]==0||!y.schub[1]==0)
    {
    schuboffset[0]=y.schub[0]+ui->offsetAbs->value();
    schuboffset[0]=(schuboffset[0]*(100+(ui->offsetPro->value())))/100;
    }
    else
    {
        qDebug()<<"Im Stand wird kein Offest hinzugefügt";
        schuboffset[0]=0;
    }
    schuboffset[1]=y.schub[1];
    schuboffset[2]=y.schub[2];
}
//ENDE Offset .................................................................

//Schubumwandlung fürs Senden .................................................
void MainWindow::schubsenden()
{
    unsigned long var;
    var=0;
    offset();

    //Hoehenmotor
    if(schuboffset[2]>99||schuboffset[2]<-99)
    {
        qDebug() << "Schub Höhenregleung MAX" << schuboffset[2];
        schuboffset[2]=schuboffset[2]/abs(schuboffset[2])*MAX;
    }
    if(schuboffset[2]<0)
    {
        var=abs(schuboffset[2])+100;
        ui->motorhoeheminus->setValue(abs(schuboffset[2]));
        ui->motorhoeheplus->setValue(0);
    }
    else
    {
        var=schuboffset[2];
        ui->motorhoeheminus->setValue(0);
        ui->motorhoeheplus->setValue(schuboffset[2]);
    }

    //Motor rechts
    if(schuboffset[1]>99||schuboffset[1]<-99)
    {
        qDebug() << "Schub Links MAX" << schuboffset[1];
        schuboffset[1]=schuboffset[1]/abs(schuboffset[1])*MAX;
    }
    if(schuboffset[1]<0)
    {
        var+=(abs(schuboffset[1])+100)*1000;
        ui->motorrechtsminus->setValue(abs(schuboffset[1]));
        ui->motorrechtsplus->setValue(0);
    }
    else
    {
        var+=schuboffset[1]*1000;
        ui->motorrechtsminus->setValue(0);
        ui->motorrechtsplus->setValue(schuboffset[1]);
    }

    //Motor links
    if(schuboffset[0]>99||schuboffset[0]<-99)
    {
        qDebug() << "Schub Links MAX" << schuboffset[0];
        schuboffset[0]=schuboffset[0]/abs(schuboffset[0])*MAX;
    }
    if(schuboffset[0]<0)
    {
        var+=(abs(schuboffset[0])+100)*1000000;
        ui->motorlinkminus->setValue(abs(schuboffset[0]));
        ui->motorlinksplus->setValue(0);
    }
    else
    {
        var+=schuboffset[0]*1000000;
        ui->motorlinkminus->setValue(0);
        ui->motorlinksplus->setValue(schuboffset[0]);}

    //Abwurf
    var+=(y.abwurfmodus+1)*1000000000;

    XbeesendCOM(var);
    //qDebug()<< "gesendetet Schubdaten:" << var;

    z.schub[0]=y.schub[0];
    z.schub[1]=y.schub[1];
}

//ENDE Schubumwandlung ........................................................

// Refreshfunktion wird durch timer ausgelöst ------------------- refresh -------------------------- refresh ----------

//Wichtig: Die Refreshfunktion ist der zentrale Taktgeber
//alle Funktionen die in einem festen Zeittackt ausgelöst werden, stehen hier drin:

void MainWindow::refresh()
{
    //qDebug()<<"REFRESH";

    DrawMap();      //KARTE loeschen und neu zeichnen

    if(ui->schubsenden->isChecked())
    {
        schubsenden();  //Aktuelle Schubwerte über XbeeCom senden
    }



    if(ui->sim->isChecked())
    {
        qDebug() << "start sim";
        z.schub[0]=y.schub[0];
        z.schub[1]=y.schub[1];
        z.sim();
        x.xList.prepend(z.xList.at(0));
        x.yList.prepend(z.yList.at(0));
        y.xList.prepend(x.xList.at(0));
        y.yList.prepend(x.yList.at(0));
        y.Ausrichtung.prepend(z.start_ausrichtung);
        y.start();
    }

    if(ui->pos_rechne->isChecked())
    {

        x.wrapper();  //Positionsbestimmung
    }

    if(ui->flug->isChecked())
    {
        y.xList.prepend(x.xList.at(0));
        y.yList.prepend(x.yList.at(0));        
        y.start(); //Regelung und Weg
    }
    if(ui->hoehe->isChecked())
    {
       y.zList.prepend(x.zList.at(0));
       y.hoehensteuerung(); //Hoehensteuerung
    }


    DrawMap();      //KARTE loeschen und neu zeichnen
}
// ENDE Refreshfunktion ------------------- ENDE refresh -------------------------- ENDE refresh ----------------------

//Handsteuerung -------------------------------------------------------------------------------------------------------

void MainWindow::vor()
{
    if(y.schub[0]<95&&y.schub[1]<95)
    {
    y.schub[0]+=2;
    y.schub[1]+=2;
    }
    qDebug() << "Key_Up:" << y.schub[0];
}

void MainWindow::back()
{
    if(y.schub[0]>-95&&y.schub[1]>-95)
    {
    y.schub[0]-=2;
    y.schub[1]-=2;
    }
    qDebug() << "Key_Down:" << y.schub[0];
}

void MainWindow::links()
{
    //geradeabweichung+=5;
    //y.geradeaus(100, geradeabweichung);

    y.schub[1]+=5;
    qDebug() << "Key_Left:" << geradeabweichung;
}

void MainWindow::rechts()
{
    //geradeabweichung-=5;
    //y.geradeaus(100, geradeabweichung);
    y.schub[0]+=6;
    qDebug() << "Key_Right:" << geradeabweichung;
}

void MainWindow::hoch()
{
    if(hoehenschubHand<98)
    {hoehenschubHand+=2;}
    y.schub[2]=hoehenschubHand;
    qDebug() << "Key_X:" << y.schub[2];
}

void MainWindow::runter()
{
    if(hoehenschubHand>-98)
    {hoehenschubHand-=2;}
    y.schub[2]=hoehenschubHand;
    qDebug() << "Key_Y:" << y.schub[2];
}

void MainWindow::stop()
{
    y.stop();
    y.schub[2]=0;
    geradeabweichung=0;
    hoehenschubHand=0;
    qDebug() << "Key_Space: Stop:" << geradeabweichung << "Schub: " << y.schub[0];
}

void MainWindow::abwurf()
{
    if(!y.abwurfmodus)
    {
        y.abwurfmodus=true;
        qDebug() << "Key_Space: Abwurf:" << y.abwurfmodus;
    }
    else
    {
        y.abwurfmodus=false;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *qkeyevent) //Tatstertur .............
{
    switch(qkeyevent->key())
    {
    case Qt::Key_Up:
        vor();
        break;
    case Qt::Key_Down:
        back();
        break;
    case Qt::Key_Right:
        rechts();
        break;
    case Qt::Key_Left:
        links();
        break;
    case Qt::Key_Space:
        stop();
        break;
    case Qt::Key_X:
        hoch();
        break;
    case Qt::Key_Y:
        runter();
        break;
    }
}
//ENDE Tastertur ..............................................................

// Test Button ---------------- Test ------------------- Test ---------------------------------------------------------
void MainWindow::onTestButtonClicked()
{
//   z.sim();
//   y.xList.prepend(z.xList.at(0));
//   y.yList.prepend(z.yList.at(0));

//   y.berechneWeg();//erste Zielkoordinate berechnen

}
// ENDE Test Button ---------------------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
    delete ui;
    delete Xbeeport;
    delete IPSport;
}



