#include "weg.h"
#include "QDebug"
#include "math.h"


//Start Konstruktor
weg::weg(){
    hinnummer = 0;
    modus = true;
    abwurfmodus = 0;
    schub[0]=0;
    schub[1]=0;
    schub[2]=0;

}
//Ende Konstruktor

//Start Aufruf
void weg::start()
{
    //int streckenlange;
    //Hier gehts los, das ist der Startpunkt für jedes mal
    if (punktabweichung(xList.at(0),yList.at(0),zielkoordinaten[0],zielkoordinaten[1])<zieltol)
    {stop();}
    else
    {
    if(notfallmodus==1)
    {notfallplan();}
    else
    {

    if (punktabweichung(xList.at(0),yList.at(0),ziel_x,ziel_y)<zieltol)
    {
        if (modus = false){
            modus = true;
            hinnummer = hinnummer + 1;
        }
        else{
            modus = false;
        }
        //Starte neue Punktberechnung
        berechneWeg();
    }

    if (modus){
        //streckenlange = punktabweichung(xList.at(0),yList.at(0),ziel_x,ziel_y);
        if(GetAbweichung()<10)
            {
            geradeaus(punktabweichung(xList.at(0),yList.at(0),ziel_x,ziel_y));
            }
            else
            {
                if(GetAbweichung()>30)
                {
                    //Notfallmodus
                }
                else
                {
                    geradeaus(punktabweichung(xList.at(0),yList.at(0),ziel_x,ziel_y),GetAbweichung());
                }
            }
    }
    }
    }
}
//Ende Aufruf

// Beginn Abweichung von idealer Flugbahn berechnen
int weg::GetAbweichung(/**int Vektor2_x, int Vektor2_y, int Vektor1_x, int Vektor1_y, int i*/)
{
    int x,y, distance, abweichung;

    switch (modus){
         case true:{
                double alpha, beta, gamma;
                int b = 1;
//                if (hinnummer == 0)
//                {
//                        AP[0]=startpunkt;  //Startpunkt festsetzen
//                        AP[1]=startpunkt;  //Startpunkt festsetzen
//                }
                x = EP[0] - AP[0];
                y = EP[1] - AP[1];
                if (x>0)
                    {b=0;}
                alpha = GetWinkel(x, y, b);
                distance = punktabweichung(AP[0],AP[1],EP[0],EP[1]);
                notfallziel[0]=x/distance;
                notfallziel[1]=y/distance;

                x = xList.at(0) - AP[0];
                y = yList.at(0) - AP[1];

                beta = GetWinkel(x, y, b);
                gamma = alpha - beta;
            //    if (gamma >= 0)
            //    {h = 0;}
            //    gamma = Abs(gamma);
                gamma = (gamma * PI)/180;


                distance = sin(gamma)*distance;
                abweichung = (distance/kreisradius[hinnummer])*100;
                notfallziel[0]=cos(gamma)*notfallziel[0]+AP[0];
                notfallziel[1]=cos(gamma)*notfallziel[1]+AP[1];
                GetWinkel(xList.at(0)-notfallziel[0],yList.at(0)-notfallziel[1],0);
             }
    case false :{
        //distance = punktabweichung(xList.at(0),hin[hinnummer][0],yList.at(0),hin[hinnummer][1]);
        distance = punktabweichung(xList.at(0),hin[hinnummer][0],yList.at(0),hin[hinnummer][1]) - kreisradius[hinnummer];
        abweichung = (distance/kreisradius[hinnummer])*100;
    }
    }

    //modus, demnach auswählen

    //ngative abweichung ist links, positive ist rechts!!!

    // Differenz Soll Ist Abwichung von der gerade (parallele!) in%

    //Differenz zwischen Soll und Ist Radius in %
    return abweichung;
}
//Ende Abweichung berechnen

//Beginn neue Zielkoordinaten berechnen
void weg::berechneWeg()
{


    double verhaeltnis=0;
    double buf3, buf4, buf1, buf2;
    double Radius;


        switch(modus)
        {
        case true:{

            if (hinanz == 0)
            {
                AP[0]=xList.at(0);//Startpunkt festsetzen
                AP[1]=xList.at(0);//Startpunkt festsetzen
                EP[0]= zielkoordinaten[0];
                EP[1]= zielkoordinaten[1];
            }

            if (hinnummer == 0)
            {
                AP[0]=xList.at(0);//Startpunkt festsetzen
                AP[1]=xList.at(0);//Startpunkt festsetzen
            }

            buf3 =  (hin[hinnummer][0] - AP[0])/2;
            buf4 =  (hin[hinnummer][1] - AP[1])/2;
            Radius = sqrt(buf1*buf1+buf2*buf2);
            buf1 = AP[0] + buf3;
            buf2 = AP[1] + buf4;
            Tangentenberechnung(buf1,buf2, AP[0], AP[1], Radius); //Eintrittspunkt in Kreis


        }
        case false:
        {
            verhaeltnis = kreisradius[hinnummer]/(kreisradius[hinnummer]+kreisradius[hinnummer+1]);
            buf1 =  (hin[hinnummer+1][0] - hin[hinnummer][0])*verhaeltnis;
            buf2 =  (hin[hinnummer+1][1] - hin[hinnummer][1])*verhaeltnis;
            Radius = sqrt(buf1*buf1+buf2*buf2);
            buf1 = hin[hinnummer][0] + buf1;
            buf2 = hin[hinnummer][1] + buf2;

            Tangentenberechnung(buf1,buf2, buf1, buf2, Radius); //Austrittspunkt



        }

    }
 //Für den Notfallplan brauche ich das notfallziel array ausgefüllt, also die Daten wo das Zeppelin im Notfall hinsteuern soll (x und y) und den Ausrichtungswinkel, zum Schluss
}
//Ende neue Zielkoordinaten berechnen


//Beginn Schnittpunktberechnung
void weg::GetCollisionPoint(double P_x, double P_y, double Q_x, double Q_y, double r1, double r2, double *res1, double *res2, double *res3, double *res4 )
{
     r1 = abs(r1);
     r2 = abs(r2);

     //Kein Schnittpunkt
     SetPoints(0);

     double abc = punktabweichung(P_x,P_y,Q_x,Q_y);

     //Kreise innerhalb
     if(abc <= abs(r1-r2))
          return;

     //Kreise ausserhalb
     if(abc > abs(r1 + r2))
          return;

     //Unendlich Schnittpunkte
     if((P_x - Q_x == 0) && (P_y - Q_y == 0))
          return;


     //ggf. Koordinaten tauschen, um Division durch Null vermeiden
     if(Q_x == P_x)
     {
          SetPoints(2);

          double a = (P_x - Q_x)/(Q_y - P_y); // =0
          double b = ( (r1*r1 - r2*r2)- (P_y*P_y - Q_y*Q_y) - (P_x*P_x - Q_x*Q_x)  )/(2*Q_y - 2*Q_y); // =((r1^2 -r2^2) - (y^2 - y^2) - 0 ) / (2 * (y - y))
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
void weg::Tangentenberechnung(double mittelx, double mittely, double Bezugspunkt_x, double Bezugspunkt_y, double rad)
{
    double x,y,x1,x2,y1,y2;//, r1, r2;
    double alpha, beta, gamma;

    GetCollisionPoint(mittelx, mittely, hin[hinnummer][0], hin[hinnummer][1], rad, kreisradius[hinnummer],&x1,&y1,&x2,&y2); //Schnittpunkte der zwei Kreise berechnen

    //Entscheidung, welcher Punkt der richtige ist, abhängig davon, ob der zeppelin rechts oder links herum fliegen soll
    int b=1;
    x= (hin[hinnummer][0] - Bezugspunkt_x);
    y= (hin[hinnummer][1] - Bezugspunkt_y);
    if (x>=0)
    {b=0;}
    alpha = GetWinkel(x,y,b);
    x=x1-Bezugspunkt_x;
    y=y1-Bezugspunkt_y;
    beta = GetWinkel(x,y,b);
//    x=x2-xList.at(0);
//    y=y2-yList.at(0);
//    gamma = GetWinkel(x,y,b);

    if (modus)
    {
        if (hin[hinnummer][2]==1) //falls linksrum
        {
            if ((beta-alpha)<0)
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
            if ((beta-alpha)<0)
            {
                AP[0]=x2;
                AP[1]=y2;
            }
            else
            {
                AP[0]=x1;
                AP[1]=y1;
            }
        }
        ziel_x = AP[0];
        ziel_y = AP[1];
    }
    //Entscheidung Ende


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
 if (i == 0)
 {
     if (y<0)
     {
     param = -param;
     result = ((acos (param) * 180.0) / PI)-180.0;
     }
     else
     {
     result = acos (param) * 180.0 / PI;
     }
 }
 else if (i==1)
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

 //cout<<"The arc cosine of "<< param << " is " << result << " degrees.\n"<<endl;
 return result;
}
//Ende Winkel

//Radien um die Stangen berechnen
void weg::berechneRadien()
{
    int i;
    int j;
    int d;
    int Abstand;
    int bef1;
    int bef2;

    for (i=0;hinanz;i++)
    {
        d = hin[i][3];
        for (j=0;hinanz;j++)
        {
            if (i!=j)
            {
                Abstand=punktabweichung(hin[i][0],hin[i][1],hin[j][0],hin[j][1]);

                if (Abstand<d)
                {
                    d=Abstand;
                    naechsterNachbar[i] = j;
                }
            }
        }
        kreisradius[i]=d/2;
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
        winkelzumziel=GetWinkel(notfallziel[0]-xList.at(0), notfallziel[1]-yList.at(0),0); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
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
        if(abs(dif)<40)
        {
            schub[2]=dif;
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
    if(streckenlaenge>1000)
    {
        schub[0]=SCHNELL;
        schub[1]=SCHNELL;
    }
    else if (streckenlaenge>500)
    {
        schub[0]=LANGSAM;
        schub[1]=LANGSAM;
    }
    else if (streckenlaenge<500)
    {
        schub[0]=SUPERLANGSAM;
        schub[1]=SUPERLANGSAM;
    }
}

void weg::geradeaus(int streckenlaenge, int abweichung)
{
    if(streckenlaenge>1000)
    {
        schub[0]=SCHNELL;
        schub[1]=SCHNELL;
        schub[0]=schub[0]-((abweichung/100)*schub[0]/3);
        schub[1]=schub[1]+((abweichung/100)*schub[1]/3);
    }
    else
    {
        schub[0]=LANGSAM;
        schub[1]=LANGSAM;
        qDebug() << "schub ohne abweichung:" << schub[0];
        schub[0]=(schub[0]*(100-abweichung))/100;
        schub[1]=(schub[1]*(100+abweichung))/100;
        qDebug() << "schub mit abweichung:" << schub[0];
    }
}


void weg::kurve(bool linksrechts, int radius)
{
    //Kurvenflugmodus
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



