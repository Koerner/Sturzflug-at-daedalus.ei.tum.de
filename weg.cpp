#include "weg.h"
#include "QDebug"
#include "math.h"

#define PI 3.14159265


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
    //int Strecke;
//    int i = 0;
//    int flug;//0=geradeaus; 1= kreis
//    double verhaeltnis=0;
//    double buf3, buf4, buf1, buf2;
//    double Radius;
//    double alpha;
//    double beta[2];
    //strecke = 0;

    //Abfrage, ob Punkt erreicht ist
//    if (xList.at(0)== EP[0] && yList(0)== EP[1])
//    {
//        flug = 1;
//    }
//    if (xList.at(0)== AP[0] && yList(0)==AP[1])
//    {
//        flug = 0;
//        strecke = strecke+1;
//    }
    //qDebug()<< "Strecke:" << strecke;
    //strecke = strecke+1;



//        switch(flug)
//        {
//        case 0:{
//            //Punkt zu Kreis-Tangente
//            buf3 =  (hin[strecke][0] - xList.at(0))/2;
//            buf4 =  (hin[strecke][1] - yList.at(0))/2;
//            Radius = sqrt(buf3*buf3+buf4*buf4);
//            buf1 = xList.at(0) + buf3;
//            buf2 = yList.at(0) + buf4;

//            Tangentenberechnung(buf1,buf2, Radius); //Eintrittspunkt in Kreis

//            //Abfrage, ob links rum oder rechtsrum
//    alpha = Winkel(buf3,buf4);
//    for (i=0,i<2,i++)
//    {
//                buf1 = S[i][0]-xList.at(0);
//                buf2 = S[i][1]-yList.at(0);
//                beta[i] = Winkel(buf1,buf2);
//    }
//    switch (buf3) {
//    case (0):
//    {
//        if (beta[0]<alpha)
//        {
//            EP[0]=S[0][0];
//            EP[1]=S[0][1];
//        }

//        break;
//    case (buf3<0):
//    case (buf3>0):
//    default:
//        break;
//    }

//            if (buf3 ==0)
//            {
//                if (buf 2 >0)
//                    alpha = 90;
//                else
//                    alpha = -90;
//            }alpha = atan(buf4 / buf3);
//            buf1 = S[0][0]-xList.at(0);
//            buf2 = S[0][1]-yList.at(0);
//            beta = atan (buf2 / buf1);


//            if (hin[strecke][2] = 0)
//            {
//                if ((S[0][1]- yList.at(0))<buf)
//                {
//                    EPunkt[0] = S[0][0];
//                    EPunkt[1] = S[0][1];
//                }
//                else
//                {
//                    EPunkt[0] = S[1][0];
//                    EPunkt[1] = S[1][1];
//                }
//            }
//            break;
//        }
//        case 1:
//        {
//            verhaeltnis = kreisradius[Strecke]/(kreisradius[Strecke]+kreisradius[Strecke+1]);
//            buf1 =  (hin[strecke+1][0] - hin[strecke][0])*verhaeltnis;
//            buf2 =  (hin[strecke+1][1] - hin[strecke][1])*verhaeltnis;
//            Radius = sqrt(buf1*buf1+buf2*buf2);
//            buf1 = hin[strecke][0] + buf1;
//            buf2 = hin[strecke][1] + buf2;

//            Tangentenberechnung(buf1,buf2, Radius); //Austrittspunkt

//            //abgrage, ob links rum oder rechts rum

//        }

//    }
 //Für den Notfallplan brauche ich das notfallziel array ausgefüllt, also die Daten wo das Zeppelin im Notfall hinsteuern soll (x und y) und den Ausrichtungswinkel, zum Schluss
}

//double weg::Winkel (double x, double y)
//{

//     double param, result;
//     x=-1;
//     y=-1;
//     param = x/sqrt(x*x+y*y);
//     if (V[1] < 0)
//     {
//         param = -param;
//         result = ((acos (param) * 180.0) / PI)-180.0;
//     }
//     else
//     {
//         result = acos (param) * 180.0 / PI;
//     }


//              //cout<<"The arc cosine of "<< param << " is " << result << " degrees.\n"<<endl;
//     return 0;
//}






double Abs(double x)
{
     if(x < 0)
          x *= -1;
     return x;
}

void GetCollisionPoint(Point P1, Point P2, double r1, double r2, Result *res)
{
     r1 = Abs(r1);
     r2 = Abs(r2);

     //Kein Schnittpunkt
     res->SetPoints(0);

     //double abc = sqrt(Abs(P2.x - P1.x)*Abs(P2.x - P1.x) + Abs(P2.y - P1.y)*Abs(P2.y - P1.y));
     double abc = sqrt((P2.x - P1.x)*(P2.x - P1.x) + (P2.y - P1.y)*(P2.y - P1.y));

     //Kreise innerhalb
     if(abc <= Abs(r1-r2))
          return;

     //Kreise ausserhalb
     if(abc > Abs(r1 + r2))
          return;

     //Unendlich Schnittpunkte
     if((P1.x - P2.x == 0) && (P1.y - P2.y == 0))
          return;


     //ggf. Koordinaten tauschen, um Division durch Null vermeiden
     if(P2.x == P1.x)
     {
          res->SetPoints(2);

          double a = (P1.x - P2.x)/(P2.y - P1.y); // =0
          double b = ( (r1*r1 - r2*r2)- (P1.y*P1.y - P2.y*P2.y) - (P1.x*P1.x - P2.x*P2.x)  )/(2*P2.y - 2*P1.y); // =((r1^2 -r2^2) - (P1.y^2 - P2.y^2) - 0 ) / (2 * (P2.y - P1.y))
          double e = a*a+1; // =1
          double f = (2*a*(b-P1.y))-(2*P1.x); // =0 -(2*P1.x)
          double g = (b-P1.y)*(b-P1.y) -r1*r1 + P1.x*P1.x;

          res->P1.x = (-f + sqrt(f*f - 4*e*g) )/(2*e);
          res->P2.x = (-f - sqrt(f*f - 4*e*g) )/(2*e);
          res->P1.y = res->P1.x * a + b; // =b
          res->P2.y = res->P2.x * a + b; // =b
     }
     else
     {
          res->SetPoints(2);

          double a = (P1.y - P2.y)/(P2.x - P1.x);
          double b = ( (r1*r1 - r2*r2)- (P1.x*P1.x - P2.x*P2.x) -  (P1.y*P1.y - P2.y*P2.y)  )/(2*P2.x - 2*P1.x);
          double e = a*a+1;
          double f = (2*a*(b-P1.x))-(2*P1.y);
          double g = (b-P1.x)*(b-P1.x) -r1*r1 + P1.y*P1.y;

          res->P1.y = (-f + sqrt(f*f - 4*e*g) )/(2*e);
          res->P2.y = (-f - sqrt(f*f - 4*e*g) )/(2*e);
          res->P1.x = res->P1.y * a + b;
          res->P2.x = res->P2.y * a + b;

     }

     if(res->P1.y == res->P2.y && res->P1.x == res->P2.x)
          res->SetPoints(1);
}

void weg::Tangentenberechnung(double mittelx, double mittely, double rad)
{
    double x,y;//, r1, r2;
    //if kreisradius[0]>2*rad
    //{
    //    error;
   // }
    x = mittelx;
    y = mittely;
    Point A(x,y);
    x = hin[0][0];
    y = hin[0][1];
    Point B(x,y);
    Result test;

    GetCollisionPoint(A,B, rad, kreisradius[0], &test);

    if(test.GetPoints() == 2)
    {     //std::cout << "Schnittpunkt S1(" << test.P1.x << "," << test.P1.y << ") S2(" << test.P2.x << "," << test.P2.y << ")" << std::endl;
        S[0][0] = test.P1.x;
        S[0][1] = test.P1.y;
        S[1][0] = test.P2.x;
        S[1][1] = test.P2.y;
    }
}

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
    schub[0]=geschwindigkeit;
    schub[1]=geschwindigkeit;
}


void weg::kurve(bool linksrechts, int radius)
{

}

void weg::standdrehung(int winkel)
{

}



