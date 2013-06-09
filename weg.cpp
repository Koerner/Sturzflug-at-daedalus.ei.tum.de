#include "weg.h"
#include "QDebug"
#include "math.h"


//Start Aufruf
void weg::start()
{
    //Hier gehts los, das ist der Startpunkt für jedes mal

    if (punktabweichung(0,0,xList.at(0),yList.at(0))<zieltol)
    {
        //Starte neue Punktberechnung
        berechneWeg();
    }
    else
    {
        if(GetAbweichung()<10)
        {
            //Passt, weiterfliegen
        }
        else
        {
            if(GetAbweichung()>30)
            {
                //Notfallmodus
            }
            else
            {
                //Reglermodus
            }
        }
    }

}
//Ende Aufruf

// Beginn Abweichung von idealer Flugbahn berechnen
int weg::GetAbweichung(/**int Vektor2_x, int Vektor2_y, int Vektor1_x, int Vektor1_y, int i*/)
{
    int x,y, distance;

    switch (modus){
         case 0:{
                double alpha, beta, gamma;
                int b = 1;
                if (strecke == 0)
                {
                        //AP[0]=Startpunkt_x;  //Startpunkt festsetzen
                        //AP[1]=Startpunkt_y;  //Startpunkt festsetzen
                }
                x = AP[0] - EP[0];
                y = AP[1] - EP[1];
                if (x>0)
                    {b=0;}
                alpha = GetWinkel(x, y, b);
                distance = sqrt(x*x+y*y);

                x = xList.at(0) - AP[0];
                y = yList.at(0) - AP[1];

                beta = GetWinkel(x, y, b);
                gamma = alpha - beta;
            //    if (gamma >= 0)
            //    {h = 0;}
            //    gamma = Abs(gamma);
                gamma = (gamma * PI)/180;


                distance = sin(gamma)*distance;
             }
    case 1 :{
        distance = (hin[strecke][0]-xList.at(0))*(hin[strecke][0]-xList.at(0))+(hin[strecke][1]-yList.at(0))*(hin[strecke][1]-yList.at(0));
        distance = sqrt(distance);
        distance = distance - kreisradius[strecke];
    }
    }



    //modus, demnach auswählen

    //ngative abweichung ist links, positive ist rechts!!!

    // Differenz Soll Ist Abwichung von der gerade (parallele!) in%

    //Differenz zwischen Soll und Ist Radius in %
    return distance;
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
        case 0:{

            if (strecke == 0)
            {
                    //AP[0]=Startpunkt_x;//Startpunkt festsetzen
                    //AP[1]=Startpunkt_y;//Startpunkt festsetzen
            }

            buf3 =  (hin[strecke][0] - AP[0])/2;
            buf4 =  (hin[strecke][1] - AP[1])/2;
            Radius = sqrt(buf3*buf3+buf4*buf4);
            buf1 = AP[0] + buf3;
            buf2 = AP[1] + buf4;
            Tangentenberechnung(buf1,buf2, AP[0], AP[1], Radius); //Eintrittspunkt in Kreis


        }
        case 1:
        {
            verhaeltnis = kreisradius[strecke]/(kreisradius[strecke]+kreisradius[strecke+1]);
            buf1 =  (hin[strecke+1][0] - hin[strecke][0])*verhaeltnis;
            buf2 =  (hin[strecke+1][1] - hin[strecke][1])*verhaeltnis;
            Radius = sqrt(buf1*buf1+buf2*buf2);
            buf1 = hin[strecke][0] + buf1;
            buf2 = hin[strecke][1] + buf2;

            Tangentenberechnung(buf1,buf2, buf1, buf2, Radius); //Austrittspunkt



        }

    }
 //Für den Notfallplan brauche ich das notfallziel array ausgefüllt, also die Daten wo das Zeppelin im Notfall hinsteuern soll (x und y) und den Ausrichtungswinkel, zum Schluss
}
//Ende neue Zielkoordinaten berechnen



//Beginn Betrag nehmen
double weg::Abs(double x)
{
     if(x < 0)
          x *= -1;
     return x;
}
//Ende Betrag nehmen

//Beginn Schnittpunktberechnung
void weg::GetCollisionPoint(double P_x, double P_y, double Q_x, double Q_y, double r1, double r2, double *res1, double *res2, double *res3, double *res4 )
{
     r1 = Abs(r1);
     r2 = Abs(r2);

     //Kein Schnittpunkt
     SetPoints(0);

     double abc = sqrt((Q_x - P_x)*(Q_x - P_x) + (Q_y - P_y)*(Q_y - P_y));

     //Kreise innerhalb
     if(abc <= Abs(r1-r2))
          return;

     //Kreise ausserhalb
     if(abc > Abs(r1 + r2))
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

    GetCollisionPoint(mittelx, mittely, hin[strecke][0], hin[strecke][1], rad, kreisradius[strecke],&x1,&y1,&x2,&y2); //Schnittpunkte der zwei Kreise berechnen

    //Entscheidung, welcher Punkt der richtige ist, abhängig davon, ob der zeppelin rechts oder links herum fliegen soll
    int b=1;
    x= (hin[strecke][0] - Bezugspunkt_x);
    y= (hin[strecke][1] - Bezugspunkt_y);
    if (x>=0)
    {b=0;}
    alpha = GetWinkel(x,y,b);
    x=x1-Bezugspunkt_x;
    y=y1-Bezugspunkt_y;
    beta = GetWinkel(x,y,b);
//    x=x2-xList.at(0);
//    y=y2-yList.at(0);
//    gamma = GetWinkel(x,y,b);

    if (modus == 0)
    {
        if (hin[strecke][2]==1) //falls linksrum
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

    }
    else
    {
        if (hin[strecke][2]==1) //falls linksrum
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
    }
    //Entscheidung Ende


}
//Ende Tangentenberechnung

int weg::punktabweichung(int x_1, int y_1, int x_2, int y_2)
{
    //Berechnet den Abtsand zweier Punkte
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
        d = hin[j][3];
        for (j=0;hinanz;j++)
        {
            if (i!=j)
            {
                bef1 = (hin[j][0]-hin[i][0]);
                bef1 = bef1*bef1;
                bef2 = (hin[j][1]-hin[i][1]);
                bef2=bef2*bef2;
                Abstand=sqrt(bef1+bef2);

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
    //Filtertimer->setInterval(400);   EVENTUELL BENÖTIGT
    if(abs(notfallziel[0]-xList.at(0))<zieltol&&abs(notfallziel[1]-yList.at(0)<zieltol))
    {
        notfallplanende(notfallziel[2]);
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
    schub[0]=geschwindigkeit;
    schub[1]=geschwindigkeit;
}


void weg::kurve(bool linksrechts, int radius)
{

}

void weg::standdrehung(int winkel)
{

}



