#ifndef STICK_H
#define STICK_H

#include "Game.h"

 class Stick
 {
 public:
 	point power;
 	point target;
 	double length;
 	double tipRadius,tailRadius;
 	double angle;
 	double force;
 	point pos;
 	void updateTarget(point);
 	void update(int,int); // direction and power
 	/* data */
 };
#endif
 