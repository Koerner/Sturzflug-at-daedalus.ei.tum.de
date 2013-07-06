#include "weg.h"
#include "QDebug"
#include "math.h"


//Start Konstruktor----------------------------------------------------------------------------------------------------
weg::weg(){
    hinnummer = 0;
    hinanz = 0;
    modus = 1;
    abwurfmodus = 0;
    schub[0]=0;
    schub[1]=0;
    schub[2]=0;
    notfallmodus=0;
    rueck_countdown=0;
}
//Ende Konstruktor-----------------------------------------------------------------------------------------------------

//Start Aufruf und Abfragen--------------------------------------------------------------------------------------------
void weg::start()
{

    int Soll_Ausrichtung,Abweichung_Ausrichtung;
    qDebug()<<"Start und Modus:"<<modus;

    if (BetragVektor(xList.at(0),yList.at(0),abwurfkoordinate[0],abwurfkoordinate[1])<zieltol)
    {
        abwurfmodus = 1;
    }
    else
    {
        abwurfmodus = 0;
    }
    if (BetragVektor(xList.at(0),yList.at(0),zielkoordinaten[0],zielkoordinaten[1])<zieltol)
    {stop();}
    else
    { //Für den Notfallplan brauche ich das notfallziel array ausgefüllt, also die Daten wo das Zeppelin im Notfall hinsteuern soll (x und y) und den Ausrichtungswinkel, zum Schluss
        if(notfallmodus==1)
        {notfallplan();}
        else
        {
            qDebug()<<"Anzahl"<<anz_rueckschub;
            qDebug()<<"Reuckcountdown"<<rueck_countdown;
            if (rueck_countdown>0)
            {

                rueck_countdown-=1;
            }
            else
            {
                if ((modus == false)&&(BetragVektor(xList.at(0),yList.at(0),ziel_x,ziel_y)<zieltol))
                {
                    modus = true;
                    hinnummer = hinnummer + 1;
                    berechneWeg();
                    rueck();
                    rueck_countdown=anz_rueckschub-1;

                }
                else if((modus==true)&&(BetragVektor(xList.at(0),yList.at(0),ziel_x,ziel_y)<zieltol))
                {
                    modus = false;
                    berechneWeg();
                    rueck();
                    rueck_countdown=anz_rueckschub-1;
                }

                else
                {

                    if (modus)
                    {
                        Soll_Ausrichtung = Runden(GetWinkel(ziel_x-AP[0],ziel_y-AP[1]));
                        Abweichung_Ausrichtung=DifferenzWinkel(Soll_Ausrichtung,Ausrichtung.at(0));
                        notfallziel[2]=Soll_Ausrichtung;
                        qDebug()<<"Abweichung"<<Berechne_Abweichung();
                        qDebug()<<"IST_Ausrichtung"<<Ausrichtung.at(0);
                        qDebug()<<"SOll_Ausrichtung"<<Soll_Ausrichtung;
                        qDebug()<<"Abweichung_Ausrichtung"<<Abweichung_Ausrichtung;
                        if(abs(Berechne_Abweichung())<100)
                        {
                            geradeaus(BetragVektor(xList.at(0),yList.at(0),ziel_x,ziel_y),Abweichung_Ausrichtung);
                        }
                        else
                        {
                            if ((Abweichung_Ausrichtung<20&&Berechne_Abweichung()<=-100)||(Abweichung_Ausrichtung>-20&&Berechne_Abweichung()>=100))
                            {
                                geradeaus(BetragVektor(xList.at(0),yList.at(0),ziel_x,ziel_y),Berechne_Abweichung());
                            }
                            else
                            {
                                geradeaus(BetragVektor(xList.at(0),yList.at(0),ziel_x,ziel_y));
                            }

                        }

                        //if(Berechne_Abweichung()>30)
                        //{
                        //    //Notfallmodus
                        // }
                        //else
                        //{

                        //}
                    }
                    else
                    {

                        qDebug()<<"Abweichung"<<Berechne_Abweichung();
                        if (hin[hinnummer][2]==0)
                        {
                            Soll_Ausrichtung = Runden(GetWinkel(xList.at(0)-hin[hinnummer][0],yList.at(0)-hin[hinnummer][1]))+90;
                            if (Soll_Ausrichtung>180){Soll_Ausrichtung = Soll_Ausrichtung-360;}
                        }
                        else
                        {
                            Soll_Ausrichtung = Runden(GetWinkel(xList.at(0)-hin[hinnummer][0],yList.at(0)-hin[hinnummer][1]))-90;
                            if (Soll_Ausrichtung<=-180){Soll_Ausrichtung = Soll_Ausrichtung+360;}
                        }
                        notfallziel[2]=Soll_Ausrichtung;
                        Abweichung_Ausrichtung=DifferenzWinkel(Soll_Ausrichtung,Ausrichtung.at(0));
                        qDebug()<<"IST_Ausrichtung"<<Ausrichtung.at(0);
                        qDebug()<<"SOll_Ausrichtung"<<Soll_Ausrichtung;
                        qDebug()<<"Abweichung_Ausrichtung"<<Abweichung_Ausrichtung;
                        if(abs(Berechne_Abweichung())<10)
                        {
                            kurve(hin[hinnummer][2],kreisradius[hinnummer]);
                        }
                        else
                        {
                            //if(Berechne_Abweichung()>30)
                            //{
                            //    //Notfallmodus
                            // }
                            //else
                            //{
                            //                    if ((Abweichung_Ausrichtung<20&&Berechne_Abweichung()<=-100)||(Abweichung_Ausrichtung>-20&&Berechne_Abweichung()>=100))
                            //                    {
                            //                        kurve(hin[hinnummer][2],kreisradius[hinnummer],Berechne_Abweichung());
                            //                    }
                            //                    else
                            //                    {
                            kurve(hin[hinnummer][2],kreisradius[hinnummer],Berechne_Abweichung());
                            //}
                            //}

                        }
                    }
                }
            }
        }
    }
}
//Ende Aufruf----------------------------------------------------------------------------------------------------------

// Beginn IST-Abweichung von idealer Flugbahn berechnen (negative Abweichung ist links von der Geraden/Kurve)--------------
// und Einspeichern des Notfallarrays
int weg::Berechne_Abweichung()
{
    int abweichung;
    double alpha, beta, gamma, distance;

    switch (modus){
    case true:{
        alpha = GetWinkel(EP[0] - AP[0], EP[1] - AP[1]);

        //Notfallmodus optimieren
        //                distance = BetragVektor(AP[0],AP[1],EP[0],EP[1]);
        //                notfallziel[0]=(EP[0] - AP[0])/distance;
        //                notfallziel[1]=(EP[1] - AP[1])/distance;

        distance = BetragVektor(AP[0],AP[1],xList.at(0),yList.at(0));
        if (distance<10)
        {
            beta = alpha;
        }
        else
        {
            beta = GetWinkel(xList.at(0) - AP[0], yList.at(0) - AP[1]);
        }
        gamma = (DifferenzWinkel(alpha,beta)* PI)/180;
        abweichung = sin(gamma)*distance;

        //Notfallmodus
        //                notfallziel[0]=cos(gamma)*notfallziel[0]+AP[0];
        //                notfallziel[1]=cos(gamma)*notfallziel[1]+AP[1];
        //                GetWinkel(xList.at(0)-notfallziel[0],yList.at(0)-notfallziel[1]);
        break;
    }
    case false :
    {
        distance = BetragVektor(hin[hinnummer][0],hin[hinnummer][1],xList.at(0),yList.at(0)) - kreisradius[hinnummer];
        if (hin[hinnummer][2]==0)
        { distance = distance*(-1);}
        abweichung = Runden(distance);
        abweichung = 0; //muss gelöscht werden
        break;
    }
    }
    return abweichung;
}
//Ende IST-Abweichung berechnen----------------------------------------------------------------------------------------

//Beginn der Wegberechnung---------------------------------------------------------------------------------------------

//Übergabe der Koordinatspunkte für Tangentenpunktberechnung..................
void weg::berechneWeg()
{
    qDebug()<<"hinderniss"<<hinnummer<<"Modus"<<modus;

    float verhaeltnis;
    double buf1, buf2;// P_x, P_y, S_x, S_y;
    double Radius;


    switch(modus)
    {
    case true:
    {

        if ((hinanz == 0)||(hinnummer==hinanz))         /* hier werden die Zielkoordinaten*/
        {                                               /* als Zielpunkt definiert*/
            if (hinanz == 0)
            {
                AP[0]=xList.at(0);//Startpunkt festsetzen
                AP[1]=yList.at(0);//Startpunkt festsetzen
            }
            EP[0]= zielkoordinaten[0];
            EP[1]= zielkoordinaten[1];
            ziel_x=EP[0];
            ziel_y=EP[1];
            qDebug()<<"Ziel:"<<ziel_x<<ziel_y;
        }
        else
        {

            if (hinnummer == 0)
            {
                AP[0]=xList.at(0);//Startpunkt festsetzen
                AP[1]=yList.at(0);//Startpunkt festsetzen
            }

            buf1 =  (hin[hinnummer][0] - AP[0])/2;              /*Mittelpunkt S berechnen*/
            buf2 =  (hin[hinnummer][1] - AP[1])/2;
            Radius = sqrt(buf1*buf1+buf2*buf2);
            buf1 = AP[0] + buf1;
            buf2 = AP[1] + buf2;
            Tangentenberechnung(buf1,buf2,AP[0],AP[1], Radius); //Eintrittspunkt in Kreis berechnen
        }
        break;
    }

    case false:
    {
        if (hinnummer+1==hinanz){                               /*letzter Austrittspunkt*/
            buf1 =  (zielkoordinaten[0] - hin[hinnummer][0])/2;
            buf2 =  (zielkoordinaten[1] - hin[hinnummer][1])/2;
            Radius = sqrt(buf1*buf1+buf2*buf2);
            buf1 = hin[hinnummer][0] + buf1;                    /*Punkt S berechnen*/
            buf2 = hin[hinnummer][1] + buf2;                    /*Punkt S berechnen*/
            Tangentenberechnung(buf1,buf2,zielkoordinaten[0],zielkoordinaten[1], Radius); //Austrittspunkt aus Kreis
        }
        else{                                                           /*innere Tangente der zwei Kreise bilden*/
            verhaeltnis = (kreisradius[hinnummer]+kreisradius[hinnummer+1]); /*Punkt P über Strahlensatz*/
            verhaeltnis = kreisradius[hinnummer]/verhaeltnis;
            qDebug()<<"Verhaeltnis"<<verhaeltnis;

            buf1 =  ((hin[hinnummer+1][0] - hin[hinnummer][0])*verhaeltnis);
            buf2 =  ((hin[hinnummer+1][1] - hin[hinnummer][1])*verhaeltnis);
            buf1 = buf1/2;
            buf2 = buf2/2;
            Radius = sqrt(buf1*buf1+buf2*buf2);
            buf1 = hin[hinnummer][0] + buf1;                    /*Punkt S berechnen*/
            buf2 = hin[hinnummer][1] + buf2;                    /*Punkt S berechnen*/

            Tangentenberechnung(buf1,buf2,buf1,buf2, Radius); //Austrittspunkt

        }


        break;
    }

    }

}
//Ende Übergabe der Koordinatspunkte für Tangentenpunktberechnung.............


//Schnittpunktbestimmung der zwei Kreise --> mögliche Tangentenpunkte.........
void weg::BestimmeSchnittpunkte(double P_x, double P_y, double Q_x, double Q_y, double r1, double r2, double *res1, double *res2, double *res3, double *res4 )
{
     r1 = abs(r1);
     r2 = abs(r2);

     //Kein Schnittpunkt
     SetPoints(0);

//     double abc = BetragVektor(P_x,P_y,Q_x,Q_y);

//     //Kreise innerhalb
//     if(abc <= abs(r1-r2))
//          return;

//     //Kreise ausserhalb
//     if(abc > abs(r1 + r2))
//          return;

//     //Unendlich Schnittpunkte
//     if((P_x - Q_x == 0) && (P_y - Q_y == 0))
//          return;


     //ggf. Koordinaten tauschen, um Division durch Null vermeiden
     if(Q_x == P_x)
     {
          SetPoints(2);

          double a = (P_x - Q_x)/(Q_y - P_y);
          double b = ( (r1*r1 - r2*r2)- (P_y*P_y - Q_y*Q_y) - (P_x*P_x - Q_x*Q_x)  )/(2*Q_y - 2*P_y); // =((r1^2 -r2^2) - (y^2 - y^2) - 0 ) / (2 * (y - y))
          double e = a*a+1;
          double f = (2*a*(b-P_y))-(2*P_x);
          double g = (b-P_y)*(b-P_y) -r1*r1 + P_x*P_x;

          *res1 = (-f + sqrt(f*f - 4*e*g) )/(2*e);
          *res3 = (-f - sqrt(f*f - 4*e*g) )/(2*e);
          *res2 = *res1 * a + b;
          *res4 = *res3 * a + b;
     }
     else
     {
          SetPoints(2);

          double a = (P_y - Q_y)/(Q_x - P_x);
          double b = ( (r1*r1 - r2*r2)- (P_x*P_x - Q_x*Q_x) -  (P_y*P_y - Q_y*Q_y)  )/(2*Q_x - 2*P_x);
          double e = a*a+1;
          double f = (2*a*(b-P_x))-(2*P_y);
          double g = (b-P_x)*(b-P_x) -r1*r1 + P_y*P_y;

          *res2 = (-f + sqrt(f*f - 4*e*g) )/(2*e);
          *res4 = (-f - sqrt(f*f - 4*e*g) )/(2*e);
          *res1 = *res2 * a + b;
          *res3 = *res4 * a + b;

     }

     if(*res2 == *res4 && *res1 == *res3)
          SetPoints(1);
}
//Ende Schnittpunktberechnung.................................................


//Beginn Tangentenpunktberechnung..................................................
void weg::Tangentenberechnung(double S_x, double S_y, double P_x, double P_y, double rad)
{
    double x1,y1; /*Schnittpunkt 1**/
    double x2,y2; /*Schnittpunkt 2**/
    double alpha,beta; /*Buffer**/

    /*Schnittpunktbestimmung**/
    BestimmeSchnittpunkte(S_x, S_y, hin[hinnummer][0], hin[hinnummer][1], rad, kreisradius[hinnummer],&x1,&y1,&x2,&y2); //Schnittpunkte der zwei Kreise berechnen

    /*Entscheidung, welcher Schnittpunkt der richtige Tangentenpunkt ist, abhängig davon,
     *ob der zeppelin rechts oder links herum fliegen soll und
     *ob es ein Austrittspunkt bzw ein Eintrittspunkt sein soll**/


    alpha = GetWinkel(hin[hinnummer][0] - P_x,hin[hinnummer][1] - P_y);
    beta = GetWinkel(x1-P_x,y1-P_y);

    qDebug()<<"Alpha"<<alpha;
    qDebug()<<"Beta"<<beta;
    qDebug()<<"Differenz"<<DifferenzWinkel(alpha,beta);
    if (modus == true) //Flug Geradeaus
    {
        if (hin[hinnummer][2]==1) //falls linksrum
        {
            if (DifferenzWinkel(alpha,beta)>0)
            {
                EP[0]=x1;
                EP[1]=y1;
            }
            else
            {
                EP[0]=x2;
                EP[1]=y2;
            }
        }
        else
        {
            if (DifferenzWinkel(alpha,beta)<0)
            {
                EP[0]=x1;
                EP[1]=y1;
            }
            else
            {
                EP[0]=x2;
                EP[1]=y2;
            }
        }
        ziel_x = EP[0]; /*Abspeichern der Zielkoordinaten**/
        ziel_y = EP[1];
    }

    else
    {
        if (hin[hinnummer][2]==1) //falls linksrum
        {
            if (DifferenzWinkel(alpha,beta)<0)
            {
                AP[0]=x1;
                AP[1]=y1;
            }
            else
            {
                AP[0]=x2;
                AP[1]=y2;
            }
        }
        else
        {
            if (DifferenzWinkel(alpha,beta)>=0)
            {
                AP[0]=x1;
                AP[1]=y1;
            }
            else
            {
                AP[0]=x2;
                AP[1]=y2;
            }
        }
        ziel_x = AP[0]; /*Abspeichern des Zwischenziels**/
        ziel_y = AP[1];
     }

    //Entscheidung Ende
    qDebug()<<"Punkt 1:"<<x1<<y1;
    qDebug()<<"Punkt 2:"<<x2<<y2;
    qDebug()<<"Zielpunkte Hindernis"<<ziel_x<<ziel_y;

}
//Ende Tangentenpunktberechnung...............................................

//Abstand zweier Vektoren bzw Betrag eines Vektors............................
int weg::BetragVektor(int x_1, int y_1, int x_2, int y_2)
{
    //Berechnet den Abstand zweier Punkte
    return sqrt(((x_2-x_1)*(x_2-x_1))+((y_2-y_1)*(y_2-y_1)));
}
//Ende Abstandsbrechnung......................................................

//Winkel eines Vektors im Koordinatensystem...................................
double weg::GetWinkel (double An, double Geg)//An: Ankathete bzw x-Koordinate, Geg: Gegenkathete bzw y-Koordinate
{
    double H; //Hypotenuse
    double Winkel;

    H=sqrt(An*An+Geg*Geg);
    if (Geg<0)
    {
        H = -H;
        Winkel =((acos (An/H) * 180.0)/PI)-180.0;
    }
    else
    {
        Winkel=acos(An/H)*180/PI;

    }
    return Winkel;
}
//Ende Winkelbestimmung.......................................................

//Berechne Differenz von Winkeln..............................................
double weg::DifferenzWinkel(double Winkel1, double Winkel2)
{
    double DifferenzWinkel1;
    DifferenzWinkel1=Winkel2-Winkel1;
    if (DifferenzWinkel1<-180)
    {
        DifferenzWinkel1 = DifferenzWinkel1+360;
    }
    else if (DifferenzWinkel1>180)
    {
        DifferenzWinkel1 = DifferenzWinkel1-360;
    }
    return DifferenzWinkel1;
}
//Ende Differenz von Winkeln..................................................

//Runden......................................................................
int weg::Runden(double Zahl)
{
    if (Zahl>=0){Zahl += 0.5;}
    else {Zahl -= 0.5;}
    return Zahl;
}
//Ende Runden.................................................................

//Radien um die Hindernisse berechnen.........................................
void weg::berechneRadien()
{
    int i,j,d;
    double Abstand;

    for (i=0;i<hinanz;i++)
    {
        d = hin[i][3];
        for (j=0;j<hinanz;j++)
        {
            if (i!=j)
            {
                Abstand=BetragVektor(hin[i][0],hin[i][1],hin[j][0],hin[j][1]);

                if (Abstand<d)
                {
                    d=Runden(Abstand);
                    naechsterNachbar[i] = j;
                }
            }
        }
        kreisradius[i]=d/2;
        qDebug()<<"Kreisradius"<<kreisradius[i];
    }
}
//Ende Radiusbestimmung.......................................................

//Ende Wegfindung------------------------------------------------------------------------------------------------------




//NOTFALLPLAN----------------------------------------------------------------------------------------------------------



void weg::notfallplan()
{
    notfallmodus=1;
    //Filtertimer->setInterval(400);   EVENTUELL BENÖTIGT
    if(BetragVektor(xList.at(0),yList.at(0),notfallziel[0],notfallziel[1])<notfalltol)
    {
        notfallplanende();
        //Filtertimer->setInterval(1000);
    }
    else
    {
        int winkelzumziel=0;
        winkelzumziel=GetWinkel(notfallziel[0]-xList.at(0), notfallziel[1]-yList.at(0)); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        if(abs(winkelzumziel-Ausrichtung.at(0))<notfalltolwinkel)
        {
            geradeaus(BetragVektor(xList.at(0),yList.at(0),notfallziel[0],notfallziel[1]));
        }
        else
        {
            notfallplananfang(winkelzumziel-Ausrichtung.at(0));
        }
    }
}

void weg::notfallplananfang(int winkel)
{
    standdrehung(winkel);
}

void weg::notfallplanende()
{
    if(notfallziel[2]-Ausrichtung.at(0)<notfalltolwinkel)
    {
        stop();
        notfallmodus=0;
    }
    else
    {
        standdrehung(notfallziel[2]-Ausrichtung.at(0));
    }
}

//Ende Notfallplan-----------------------------------------------------------------------------------------------------


//Steuerung------------------------------------------------------------------------------------------------------------

// Hoehensteuerung............................................................

void weg::hoehensteuerung()
{
    double dif1, dif2, dif3;
    dif1=sollHoehe-zList.at(0);
    dif2=zList.at(hoehe_d)-zList.at(0);
    dif3=sollHoehe-zList.at(hoehe_d);
    if ((abs(dif3)>hoehentol)&&(dif3<0)&&(dif2>0))
    {
        schub[2]=dif2/(HOEHENSCHUBMULTIPLIKATOR);//*(rueckschub);
    }
    else if ((abs(dif3)>hoehentol)&&(dif3>0)&&(dif2<0))
    {
        schub[2]=dif2/(HOEHENSCHUBMULTIPLIKATOR);//*(rueckschub);
    }
    else
    {
        if (abs(dif1)<hoehentol)
        {
            schub[2]=0;
        }
        else
        {
            if(abs(dif1)<800)
            {
                schub[2]=dif1/HOEHENSCHUBMULTIPLIKATOR;
            }
            else
            {
                if(dif1<0)
                {schub[2]=-MAX;}
                else
                {schub[2]=MAX;}
            }
        }
    }

}
//............................................................................

//STEUERUNG linker/rechter Motor..............................................

//Geradeaus>>>>>>>>>>>>>>>>>>
void weg::geradeaus(int streckenlaenge)
{

    if(streckenlaenge>400)
    {
        schub[0]=SCHNELL;
        schub[1]=SCHNELL;
    }
    else if (streckenlaenge>=200)
    {
        schub[0]=LANGSAM;
        schub[1]=LANGSAM;
    }
    else if (streckenlaenge<200)
    {
        schub[0]=SUPERLANGSAM;
        schub[1]=SUPERLANGSAM;
    }
    qDebug()<<"Gerade ohne Regelung:"<<schub[0]<<schub[1];
}

void weg::geradeaus(int streckenlaenge, double abweichung)
{
    double schu;
    if(streckenlaenge>1000)
    {
        schu=SCHNELL;
        abweichung = abweichung/3;
    }
    else if (streckenlaenge>=200)
    {
        schu=LANGSAM;
        //schub[1]=LANGSAM;
    }
    else if (streckenlaenge<200)
    {
        schu=SUPERLANGSAM;
        //schub[1]=SUPERLANGSAM;
    }
    if (abweichung<0)
    {
        schub[0]=Runden((schu*(100-abweichung))/100);
        schub[1]=schu;
    }
    else
    {
        schub[0]=schu;
        schub[1]=Runden((schu*(100+abweichung))/100);
    }
    qDebug()<<"Gerade mit Regelung:"<<schub[0]<<schub[1];
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//Kurvenflug>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void weg::kurve(int linksrechts, double radius)
{
    //Kurvenflugmodus
    if (linksrechts == 0){
        schub[1]=SUPERLANGSAM;
        schub[0]=schub[1]*((radius+spannweite)/(radius-spannweite));
        qDebug() << "Rechtskurve:" << schub[0]<< schub[1];
    }
    else{
        schub[0]=SUPERLANGSAM;
        schub[1]=schub[0]*((radius+spannweite)/(radius-spannweite));
        qDebug() << "Linkskurve:" << schub[0]<< schub[1];
    }
}

void weg::kurve(int linksrechts, int radius, int abweichung)
{
    double schu1,schu2;
    schu1=SUPERLANGSAM;
    schu2=schu1*((radius+spannweite)/(radius-spannweite));
    //Kurvenflugmodus
    if (linksrechts == 0){
        if (abweichung<0)
        {
            schub[0]=Runden((schu2*(100-abweichung))/100);
            schub[1]=schu1;
        }
        else
        {
            schub[0]=schu2;
            schub[1]=Runden((schu1*(100+abweichung))/100);
        }
        qDebug() << "Rechtskurve mit Regelung:" << schub[0]<< schub[1];
    }
    else{
        if (abweichung<0)
        {
            schub[1]=Runden((schu1*(100-abweichung))/100);
            schub[0]=schu2;
        }
        else
        {
            schub[1]=schu1;
            schub[0]=Runden((schu2*(100+abweichung))/100);
        }
        qDebug() << "Linkskurve mit Regelung:" << schub[0]<< schub[1];
    }
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void weg::standdrehung(int winkel)
{
    schub[0]=winkel/3.6;
    schub[1]=-winkel/3.6;
}

void weg::rueck()
{
    schub[0]=rueckschub*(schub[0]);
    schub[1]=rueckschub*(schub[1]);
    qDebug()<<"Rueckschub";
}

void weg::stop()
{
    schub[0]=0;
    schub[1]=0;
}

// Ende STEUERUNG linker/rechter Motor........................................

// Ende Steuerung------------------------------------------------------------------------------------------------------
