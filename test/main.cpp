#include <iostream>
#include <math.h>

int main//class Point
//{
//     public:

//     Point()
//     {
//          x = y = 0;
//     }

//     Point(double x_, double y_)
//     {
//          x = x_;
//          y = y_;
//     }

//     double x;
//     double y;
//};

//class Result
//{
//     public:

//     Result()
//     {
//          Points = 0;
//     }

//     void SetPoints(int p)
//     {
//          Points = p;
//     }

//     int GetPoints()
//     {
//          return Points;
//     }

//     Point P1;
//     Point P2;

//     private:

//     int Points;
//};

//double Abs(double x)
//{
//     if(x < 0)
//          x *= -1;
//     return x;
//}

//void GetCollisionPoint(Point P1, Point P2, double r1, double r2, Result *res)
//{
//     r1 = Abs(r1);
//     r2 = Abs(r2);

//     //Kein Schnittpunkt
//     res->SetPoints(0);


///* 01.04.2011
// * Beträge ergeben sich durch das Quadrieren, und müssen deshalb nicht nochmal extra berechnet werden
// * Mox Nöltner
//*/
//     //double abc = sqrt(Abs(P2.x - P1.x)*Abs(P2.x - P1.x) + Abs(P2.y - P1.y)*Abs(P2.y - P1.y));
//     double abc = sqrt((P2.x - P1.x)*(P2.x - P1.x) + (P2.y - P1.y)*(P2.y - P1.y));

//     //Kreise innerhalb
//     if(abc <= Abs(r1-r2))
//          return;

//     //Kreise ausserhalb
//     if(abc > Abs(r1 + r2))
//          return;

//     //Unendlich Schnittpunkte
//     if((P1.x - P2.x == 0) && (P1.y - P2.y == 0))
//          return;

///* 25.09.2007
// * Koordinatentausch war fehlerhaft
// * Jens Ziegler
//*/

//     //ggf. Koordinaten tauschen, um Division durch Null vermeiden
//     if(P2.x == P1.x)
//     {
//          res->SetPoints(2);

//          double a = (P1.x - P2.x)/(P2.y - P1.y); // =0
//          double b = ( (r1*r1 - r2*r2)- (P1.y*P1.y - P2.y*P2.y) - (P1.x*P1.x - P2.x*P2.x)  )/(2*P2.y - 2*P1.y); // =((r1^2 -r2^2) - (P1.y^2 - P2.y^2) - 0 ) / (2 * (P2.y - P1.y))
//          double e = a*a+1; // =1
//          double f = (2*a*(b-P1.y))-(2*P1.x); // =0 -(2*P1.x)
//          double g = (b-P1.y)*(b-P1.y) -r1*r1 + P1.x*P1.x;

//          res->P1.x = (-f + sqrt(f*f - 4*e*g) )/(2*e);
//          res->P2.x = (-f - sqrt(f*f - 4*e*g) )/(2*e);
//          res->P1.y = res->P1.x * a + b; // =b
//          res->P2.y = res->P2.x * a + b; // =b
//     }
//     else
//     {
//          res->SetPoints(2);

//          double a = (P1.y - P2.y)/(P2.x - P1.x);
//          double b = ( (r1*r1 - r2*r2)- (P1.x*P1.x - P2.x*P2.x) -  (P1.y*P1.y - P2.y*P2.y)  )/(2*P2.x - 2*P1.x);
//          double e = a*a+1;
//          double f = (2*a*(b-P1.x))-(2*P1.y);
//          double g = (b-P1.x)*(b-P1.x) -r1*r1 + P1.y*P1.y;

//          res->P1.y = (-f + sqrt(f*f - 4*e*g) )/(2*e);
//          res->P2.y = (-f - sqrt(f*f - 4*e*g) )/(2*e);
//          res->P1.x = res->P1.y * a + b;
//          res->P2.x = res->P2.y * a + b;

//     }

//     if(res->P1.y == res->P2.y && res->P1.x == res->P2.x)
//          res->SetPoints(1);
///* 01.04.2011
// * Schließende Klammer fehlte
// * Mox Nöltner
//*/
//}

//int main(int args, char* argv[])
//{
//     std::cout << "Geben Sie einen Punkt ein.\nx:";

//     double x,y, r1, r2;

//     std::cin >> x;
//     std::cout << "y:";
//     std::cin >> y;
//     std::cout << "Radius:";
//     std::cin >> r1;

//     Point A(x,y);

//     std::cout << std::endl << "Geben Sie den zweiten Punkt ein.\nx:";

//     std::cin >> x;
//     std::cout << "y:";
//     std::cin >> y;
//     std::cout << "Radius:";
//     std::cin >> r2;

//     Point B(x,y);

//     std::cout << std::endl << "Ihre Punkte: P(" << A.x << "," << A.y << ") r=" << r1 << " Q(" << B.x << "," << B.y << ") r=" << r2 << std::endl;

//     Result test;

//     GetCollisionPoint(A,B, r1, r2, &test);

//     std::cout << "Schnittpunkt: " << test.GetPoints() << std::endl;

//     if(test.GetPoints() == 2)
//          std::cout << "Schnittpunkt S1(" << test.P1.x << "," << test.P1.y << ") S2(" << test.P2.x << "," << test.P2.y << ")" << std::endl;
//     else if(test.GetPoints() == 1)
//          std::cout << "Schnittpunkt S(" << test.P1.x << "," << test.P1.y << ")" << std::endl;

//     return 0;
//}
