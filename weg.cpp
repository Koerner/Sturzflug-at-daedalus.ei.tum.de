#include "weg.h"
#include "QDebug"
#include "math.h"


//Start Konstruktor
weg::weg(){
    hinnummer = 0;
    hinanz = 0;
    modus = 0;
    abwurfmodus = 0;
    schub[0]=0;
    schub[1]=0;
    schub[2]=0;
    notfallmodus=0;

}
//Ende Konstruktor

//Start Aufruf
void weg::start()
{
    //int streckenlange;
    //Hier gehts los, das ist der Startpunkt für jedes mal
    qDebug()<<"Start und Modus:"<<modus;
    if (punktabweichung(xList.at(0),yList.at(0),zielkoordinaten[0],zielkoordinaten[1])<zieltol)
    {stop();}
    else
    {
        if(notfallmodus==1)
        {notfallplan();}
        else
        {
            if ((modus == false)&&(punktabweichung(xList.at(0),yList.at(0),ziel_x,ziel_y)<zieltol))
            {
                modus = true;
                hinnummer = hinnummer + 1;
                berechneWeg();
            }
            else if((modus==true)&&(punktabweichung(xList.at(0),yList.at(0),ziel_x,ziel_y)<zieltol))
            {
                modus = false;
                berechneWeg();
            }

            int Abweichung_Ausrichtung;
            if (modus)
            {
                Abweichung_Ausrichtung=Ausrichtung.at(0)-Runden(GetWinkel(ziel_x-AP[0],ziel_y-AP[1],ziel_x-AP[0]));
                if(abs(Berechne_Abweichung())<100)
                {
                    geradeaus(punktabweichung(xList.at(0),yList.at(0),ziel_x,ziel_y),Abweichung_Ausrichtung);
                }
                else
                {
                    if ((Abweichung_Ausrichtung<20&&Berechne_Abweichung()<-100)||(Abweichung_Ausrichtung>-20&&Berechne_Abweichung()>100))
                    {
                        geradeaus(punktabweichung(xList.at(0),yList.at(0),ziel_x,ziel_y),Berechne_Abweichung());
                    }
                    else
                    {
                        geradeaus(punktabweichung(xList.at(0),yList.at(0),ziel_x,ziel_y));
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
                int Soll_Ausrichtung;
                qDebug()<<"Abweichung"<<Berechne_Abweichung();
                if (hin[hinnummer][2]==0)
                {
                    Soll_Ausrichtung = Runden(GetWinkel(xList.at(0)-hin[hinnummer][0],yList.at(0)-hin[hinnummer][1],xList.at(0)-hin[hinnummer][0]))-90;
                }
                Abweichung_Ausrichtung=Ausrichtung.at(0)- Soll_Ausrichtung;
                qDebug()<<"IST_Ausrichtung"<<Ausrichtung.at(0);
                qDebug()<<"SOll_Ausrichtung"<<Soll_Ausrichtung;
                qDebug()<<"Abweichung_Ausrichtung"<<Abweichung_Ausrichtung;
                if(abs(Berechne_Abweichung())<=10)
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
                    qDebug()<<"Regeln";
                    kurve(hin[hinnummer][2],kreisradius[hinnummer],Berechne_Abweichung());
                    //}
                }
            }
        }
    }
}
//Ende Aufruf

// Beginn Abweichung von idealer Flugbahn berechnen (negative Abweichung ist links von der Geraden)
int weg::Berechne_Abweichung()
{
    int abweichung;
    double alpha, beta, gamma, distance;

    switch (modus){
         case true:{
                //double alpha, beta, gamma,distance;
//                if (hinnummer == 0)
//                {
//                        AP[0]=startpunkt;  //Startpunkt festsetzen
//                        AP[1]=startpunkt;  //Startpunkt festsetzen
//                }
                alpha = GetWinkel(EP[0] - AP[0], EP[1] - AP[1], EP[0] - AP[0]);
                distance = punktabweichung(AP[0],AP[1],EP[0],EP[1]);
                notfallziel[0]=(EP[0] - AP[0])/distance;
                notfallziel[1]=(EP[1] - AP[1])/distance;

                distance = punktabweichung(AP[0],AP[1],xList.at(0),yList.at(0));
                if (distance<10)
                {
                    beta = alpha;
                }
                else
                {
                beta = GetWinkel(xList.at(0) - AP[0], yList.at(0) - AP[1], EP[0] - AP[0]);
                }
                gamma = ((beta-alpha)* PI)/180;
                abweichung = sin(gamma)*distance;

                //abweichung = (distance/500)*100;
                notfallziel[0]=cos(gamma)*notfallziel[0]+AP[0];
                notfallziel[1]=cos(gamma)*notfallziel[1]+AP[1];
                GetWinkel(xList.at(0)-notfallziel[0],yList.at(0)-notfallziel[1],xList.at(0)-notfallziel[0]);
                break;
             }
    case false :
    {
        distance = punktabweichung(hin[hinnummer][0],hin[hinnummer][1],xList.at(0),yList.at(0)) - kreisradius[hinnummer];
        if (hin[hinnummer][2]==0)
        { distance = distance*(-1);}
        abweichung = Runden(distance);
        break;
    }
    }

    // Differenz Soll Ist Abwichung von der gerade (parallele!) in%

    //Differenz zwischen Soll und Ist Radius in %
    //qDebug()<<"Abweichung:"<<abweichung;
    return abweichung;
}
//Ende Abweichung berechnen

//Beginn neue Zielkoordinaten berechnen
void weg::berechneWeg()
{
    qDebug()<<"hStart_Berechne";
    qDebug()<<"hindernisse"<<hinnummer<<"Modus"<<modus;

    float verhaeltnis;
    double buf1, buf2;
    double Radius;


    switch(modus)
    {
    case true:
    {

        if ((hinanz == 0)||(hinnummer==hinanz))
        {
            if (hinanz == 0)
            {
            AP[0]=xList.at(0);//Startpunkt festsetzen
            AP[1]=yList.at(0);//Startpunkt festsetzen
            }
            EP[0]= zielkoordinaten[0];
            EP[1]= zielkoordinaten[1];
            ziel_x=EP[0];
            ziel_y=EP[1];
            qDebug()<<"Ziel"<<ziel_x<<ziel_y;
        }
        else
        {

            if (hinnummer == 0)
            {
                AP[0]=xList.at(0);//Startpunkt festsetzen
                AP[1]=yList.at(0);//Startpunkt festsetzen
            }

            buf1 =  (hin[hinnummer][0] - AP[0])/2;
            buf2 =  (hin[hinnummer][1] - AP[1])/2;
            Radius = sqrt(buf1*buf1+buf2*buf2);
            buf1 = AP[0] + buf1;
            buf2 = AP[1] + buf2;
            Tangentenberechnung(buf1,buf2,AP[0],AP[1], Radius); //Eintrittspunkt in Kreis
        }
    break;
    }

    case false:
    {
        if (hinnummer+1==hinanz){
            buf1 =  (zielkoordinaten[0] - hin[hinnummer][0])/2;
            buf2 =  (zielkoordinaten[1] - hin[hinnummer][1])/2;
            Radius = sqrt(buf1*buf1+buf2*buf2);
            buf1 = hin[hinnummer][0] + buf1;
            buf2 = hin[hinnummer][1] + buf2;
            Tangentenberechnung(buf1,buf2,zielkoordinaten[0],zielkoordinaten[1], Radius); //Austrittspunkt aus Kreis
        }
        else{
            verhaeltnis = (kreisradius[hinnummer]+kreisradius[hinnummer+1]);
            verhaeltnis = kreisradius[hinnummer]/verhaeltnis;
            qDebug()<<"Verhaeltnis"<<verhaeltnis;

            buf1 =  ((hin[hinnummer+1][0] - hin[hinnummer][0])*verhaeltnis);
            buf2 =  ((hin[hinnummer+1][1] - hin[hinnummer][1])*verhaeltnis);
            buf1 = buf1/2;
            buf2 = buf2/2;
            Radius = sqrt(buf1*buf1+buf2*buf2);
            buf1 = hin[hinnummer][0] + buf1;
            buf2 = hin[hinnummer][1] + buf2;

            Tangentenberechnung(buf1,buf2,buf1,buf2, Radius); //Austrittspunkt

        }


        break;
    }

    }
 //Für den Notfallplan brauche ich das notfallziel array ausgefüllt, also die Daten wo das Zeppelin im Notfall hinsteuern soll (x und y) und den Ausrichtungswinkel, zum Schluss
}
//Ende neue Zielkoordinaten berechnen


//Beginn Tangentenpunktberechnung
void weg::GetCollisionPoint(double P_x, double P_y, double Q_x, double Q_y, double r1, double r2, double *res1, double *res2, double *res3, double *res4 )
{
     r1 = abs(r1);
     r2 = abs(r2);

     //Kein Schnittpunkt
     SetPoints(0);

//     double abc = punktabweichung(P_x,P_y,Q_x,Q_y);

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

          double a = (P_x - Q_x)/(Q_y - P_y); // =0
          double b = ( (r1*r1 - r2*r2)- (P_y*P_y - Q_y*Q_y) - (P_x*P_x - Q_x*Q_x)  )/(2*Q_y - 2*P_y); // =((r1^2 -r2^2) - (y^2 - y^2) - 0 ) / (2 * (y - y))
          double e = a*a+1; // =1
          double f = (2*a*(b-P_y))-(2*P_x); // =0 -(2*x)
          double g = (b-P_y)*(b-P_y) -r1*r1 + P_x*P_x;

          *res1 = (-f + sqrt(f*f - 4*e*g) )/(2*e);
          *res3 = (-f - sqrt(f*f - 4*e*g) )/(2*e);
          *res2 = *res1 * a + b; // =b
          *res4 = *res3 * a + b; // =b
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
//Ende Schnittpunktberechnung


//Beginn Tangentenberechnung
void weg::Tangentenberechnung(double mittelx, double mittely, double bezugspunktx, double bezugspunkty, double rad)
{
    double x1,x2,y1,y2;//, r1, r2;
    double alpha, beta;// gamma;

    GetCollisionPoint(mittelx, mittely, hin[hinnummer][0], hin[hinnummer][1], rad, kreisradius[hinnummer],&x1,&y1,&x2,&y2); //Schnittpunkte der zwei Kreise berechnen
    qDebug()<<"Punkt1"<<x1<<y1;
    qDebug()<<"Punkt2"<<x2<<y2;

    //Entscheidung, welcher Punkt der richtige ist, abhängig davon, ob der zeppelin rechts oder links herum fliegen soll
    alpha = GetWinkel(hin[hinnummer][0] - bezugspunktx,hin[hinnummer][1] - bezugspunkty,hin[hinnummer][0] - bezugspunktx);
    qDebug()<<"alpha"<<alpha;
    beta = GetWinkel(x1-bezugspunktx,y1-bezugspunkty,hin[hinnummer][0] - bezugspunktx);
    qDebug()<<"beta"<<beta;
//    x=x2-xList.at(0);
//    y=y2-yList.at(0);
//    gamma = GetWinkel(x,y,,hin[hinnummer][0] - bezugspunktx);
    if (modus == true)
    {
        if (hin[hinnummer][2]==1) //falls linksrum
        {
            if ((alpha-beta)<0)
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
            if ((alpha-beta)>0)
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
        ziel_x = EP[0];
        ziel_y = EP[1];
    }

    else
    {
        if (hin[hinnummer][2]==1) //falls linksrum
        {
            if ((alpha-beta)>0)
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
            if ((alpha-beta)<0)
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
        ziel_x = AP[0];
        ziel_y = AP[1];
     }

    //Entscheidung Ende
    qDebug()<<"Zielpunkte Hindernis"<<ziel_x<<ziel_y;

}
//Ende Tangentenberechnung

int weg::punktabweichung(int x_1, int y_1, int x_2, int y_2)
{
    //Berechnet den Abstand zweier Punkte
    return sqrt(((x_2-x_1)*(x_2-x_1))+((y_2-y_1)*(y_2-y_1)));
}


//Winkel zwischen zwei Geraden
double weg::GetWinkel (double x, double y, int i)
{
    //double V[2];
    double param;
    double result;
    //V[0]=0.5;
    //V[1]=0.5;
    param = x/sqrt(x*x+y*y);

    if (i >= 0)
    {
        if (y<0)
        {
            param = -param;
            result = ((acos (param) * 180.0) / PI)-180.0;
        }
        else
        {
            result = acos (param) * (180.0 / PI);

        }
    }
    else
    {
        if (y<0)
        {
            param = -param;
            result = ((acos (param) * 180.0) / PI)+180.0;
        }
        else
        {
            result = acos (param) * 180.0 / PI;
        }
    }

    //param = -(1/sqrt(2));
    return result;
}
//Ende Winkel

//Runden
int weg::Runden(double Zahl)
{
    if (Zahl>=0){Zahl += 0.5;}
    else {Zahl -= 0.5;}
    return Zahl;
}

//Radien um die Stangen berechnen
void weg::berechneRadien()
{
    int i;
    int j;
    int d;
    double Abstand;

    for (i=0;i<hinanz;i++)
    {
        d = hin[i][3];
        for (j=0;j<hinanz;j++)
        {
            if (i!=j)
            {
                Abstand=punktabweichung(hin[i][0],hin[i][1],hin[j][0],hin[j][1]);

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







//NOTFALLPLAN



void weg::notfallplan()
{
    notfallmodus=1;
    //Filtertimer->setInterval(400);   EVENTUELL BENÖTIGT
    if(punktabweichung(xList.at(0),yList.at(0),notfallziel[0],notfallziel[1])<notfalltol)
    {
        notfallplanende();
        //Filtertimer->setInterval(1000);
    }
    else
    {
        int winkelzumziel=0;
        winkelzumziel=GetWinkel(notfallziel[0]-xList.at(0), notfallziel[1]-yList.at(0),notfallziel[0]-xList.at(0)); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        if(abs(winkelzumziel-Ausrichtung.at(0))<notfalltolwinkel)
        {
            geradeaus(punktabweichung(xList.at(0),yList.at(0),notfallziel[0],notfallziel[1]));
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

// HOEHENSTEUERUNG

void weg::hoehensteuerung()
{
    int dif;
    dif=sollHoehe-zList.at(0);
    if (abs(dif)<hoehentol)
    {
        schub[2]=0;
    }
    else
    {
        if(abs(dif)<400)
        {
            schub[2]=dif/5;
        }
        else
        {
            if(dif<0)
            {schub[2]=-MAX;}
            else
            {schub[2]=MAX;}
        }
    }

}


// MOTORSTEUERUNG



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

void weg::geradeaus(int streckenlaenge, int abweichung)
{
    double schu;
//    if(streckenlaenge>1000)
//    {
//        schub[0]=SCHNELL;
//        schub[1]=SCHNELL;
//        //schub[0]=(schub[0]*(100-abweichung/3))/100;
//        //schub[1]=(schub[1]*(100+abweichung/3))/100;
//    }
    if (streckenlaenge>=200)
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
            schub[0]=schu1;
            schub[1]=Runden((schu2*(100+abweichung))/100);
        }
        qDebug() << "Rechtskurve mit Regelung:" << schub[0]<< schub[1];
    }
    else{
        if (abweichung<0)
        {
            schub[1]=Runden((schu2*(100-abweichung))/100);
            schub[0]=schu1;
        }
        else
        {
            schub[1]=schu1;
            schub[0]=Runden((schu2*(100+abweichung))/100);
        }
        qDebug() << "Linkskurve mit Regelung:" << schub[0]<< schub[1];
    }
}

void weg::standdrehung(int winkel)
{
    schub[0]=winkel/3.6;
    schub[1]=-winkel/3.6;
}

void weg::stop()
{
    schub[0]=0;
    schub[1]=0;
}



