 #ifndef TABLE_H
#define TABLE_H

#include <vector>
 #include "Game.h"

using namespace std;

 class Table
 {
 public:
 	point p1,p2,p3,p4;	// inner boundary
 	double len,bre;
 	double thickness; // may be 5 % of length
 	double height;
 	Table(double,double);		// the +ve +ve point
 };
#endif
