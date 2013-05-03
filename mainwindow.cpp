#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // GUI Einstellunegn

    setWindowTitle(tr("ComPort Sturzflug@daedalus"));

    //PortBox ComPort Einstellungen
        //COM Ports suchen
        foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        ui->portBox->addItem(info.portName);
        //selber eintargen Möglich machen
        ui->portBox->setEditable(true);
    //Port Box STOP

    //BaudRateBox kompatible Werte mit Win und Linux
    ui->BaudRateBox->addItem("1200", BAUD1200);
    ui->BaudRateBox->addItem("2400", BAUD2400);
    ui->BaudRateBox->addItem("4800", BAUD4800);
    ui->BaudRateBox->addItem("9600", BAUD9600);
    ui->BaudRateBox->addItem("19200", BAUD19200);
    ui->BaudRateBox->setCurrentIndex(3);
    // BaudRateBox STOP

    //ComText Box
    ui->ComText->setReadOnly(true);
    //ComText Box STOP

    timer = new QTimer(this);
    timer->setInterval(40);

    //Vordefinierte Einstelluneg
    PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    port = new QextSerialPort(ui->portBox->currentText(), settings, QextSerialPort::Polling);
    //Vordefinierte Einstellungen STOP

    //enumerator = new QextSerialEnumerator(this);
    //enumerator->setUpNotifications();
    connect(ui->BaudRateBox, SIGNAL(currentIndexChanged(int)), SLOT(onBaudRateChanged(int)));
    connect(ui->portBox, SIGNAL(editTextChanged(QString)), SLOT(onPortNameChanged(QString)));
    connect(ui->TestButton, SIGNAL(clicked()), SLOT(onTestButtonClicked()));
    connect(timer, SIGNAL(timeout()), SLOT(onReadyRead()));
    connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));



}

 //Funktionen GUI
void MainWindow::onBaudRateChanged(int idx)
{
    port->setBaudRate((BaudRateType)ui->BaudRateBox->itemData(idx).toInt());
}


void MainWindow::onPortAddedOrRemoved()
{
    QString current = ui->portBox->currentText();

    ui->portBox->blockSignals(true);
    ui->portBox->clear();
    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        ui->portBox->addItem(info.portName);

    ui->portBox->setCurrentIndex(ui->portBox->findText(current));

    ui->portBox->blockSignals(false);
}

void MainWindow::writeComText(QString writeComText)
{
    ui->ComText->moveCursor(QTextCursor::End);
    ui->ComText->insertPlainText(writeComText);
    ui->ComText->moveCursor(QTextCursor::End);
}

void MainWindow::onPortNameChanged(const QString & /*name*/)
{
    // ggf. offenen Comport schließen

    if (port->isOpen())
    {
        port->close();
        writeComText("---\nCom Port geschlossen\n");
    }
    else{}
    // STOP

    // Com Port setzen und öffnen
     port->setPortName(ui->portBox->currentText());  //Com setzen
     port->setQueryMode(QextSerialPort::Polling); //Pollin Mode einstellen
     port->open(QIODevice::ReadWrite);

     if (port->isOpen()) { //Abfrage ob's geklappt hat
         writeComText("---\nCom Port offen\n---\n");
        }
        else {
         writeComText("---\nFehler - konnte Com Port nicht oeffnen \n---\n");
              }
     //STOP
        //Da polling mode, Ein QTimer
        if (port->isOpen() && port->queryMode() == QextSerialPort::Polling)
        timer->start();
        else
        timer->stop();
     }

// ComPort lesen
void MainWindow::onReadyRead()
{
    if (port->bytesAvailable()) {
        writeComText ("->");
        writeComText(QString::fromLatin1(port->readAll()));
        writeComText ("\n");
    }
}
//ComPort lesen STOP

//ComPort schreiben
void MainWindow::sendCOM(int sendCOM)
{
    if (port->isOpen() )
    {
        QString str;
        str.append(QString("%1").arg(sendCOM));
        ui->sendEdit->insertPlainText(str);
        port->write(ui->sendEdit->toPlainText().toLatin1());
        // Textausgabe des gesendetetn
        writeComText("<-");
        writeComText(ui->sendEdit->toPlainText().toLatin1());
        writeComText("\n");
        ui->sendEdit->clear ();//loeschen sonst wird das alte immer wieder mitgesendet
    }
    else
    {
        writeComText("---\nPort nicht offen, Senden Fehlgeschlagen\n---\n");
    }

}

//ComPort schreiben STOP

//TestButton
void MainWindow::onTestButtonClicked()
{
    int Test = 101000;
   sendCOM(Test);
}
//TestButton STOP



//Funktionen GUI STOP






MainWindow::~MainWindow()
{

    delete ui;
    delete port;
}
