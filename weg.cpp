#include "weg.h"
#include "QDebug"
# include "math.h"

void weg::moduscheck()
{
    // Prüfen ob normaler Modus oder Notfallmodus
    if (!modus)
    {
        berechneWeg();
    }
    else
    {
        qDebug()<< "Notfallplan";
        notfallplan();
    }
}

void weg::berechneWeg()
{

}

int weg::Abweichung(int Radius, int Hindernisnummer)
{
    int Abweichungx=0;
    int Abweichungy=0;
    int Abweichung=0;
    Abweichungx=(xList.at(0)-hin[Hindernisnummer][0]);
    Abweichungy=(yList.at(0)-hin[Hindernisnummer][1]);
    Abweichung=Radius-sqrt((Abweichungx*Abweichungx)+(Abweichungy*Abweichungy));
    return Abweichung;
}


//Für den Notfallplan brauche ich zusätzlich das notfallziel array ausgefüllt

//NOTFALLPLAN



void weg::notfallplan()
{
    //Filtertimer->setInterval(400);   EVENTUELL BENÖTIGT
    if(abs(notfallziel[0]-xList.at(0))<zieltol&&abs(notfallziel[1]-yList.at(0)<zieltol))
    {
        notfallplanende(notfallziel[2]);
        modus=false;
        //Filtertimer->setInterval(1000);
    }
    else
    {
        int winkelzumziel=0;
        //Berechnung Winkel nach 0 von Zeppelin zu ziel
        if(abs(winkelzumziel-Ausrichtung.at(0))<1)
        {
            geradeaus(10);
        }
        else
        {
            notfallplananfang(winkelzumziel-Ausrichtung.at(0));
        }
    }




}

void weg::notfallplananfang(int AnfangsSollwinkel)
{
    standdrehung(AnfangsSollwinkel-Ausrichtung.at(0));
}

void weg::notfallplanende(int EndSollwinkel)
{
    standdrehung(EndSollwinkel-Ausrichtung.at(0));
}




// MOTORSTEUERUNG



void weg::geradeaus(int geschwindigkeit)
{
}


void weg::kurve(bool linksrechts, int radius)
{
}

void weg::standdrehung(int winkel)
{
}



