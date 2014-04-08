#include "Table.h"

Table::Table(double l, double b){
	len = l;
	bre = b;
	p1.x = b/2; p1.z = l/2;
	p2.x = -b/2; p2.z = l/2;
	p3.x = -b/2; p3.z = -l/2;
	p4.x = b/2; p4.z = -l/2;
	thickness = 0.1*l;
	height = 0.05*b;
}
