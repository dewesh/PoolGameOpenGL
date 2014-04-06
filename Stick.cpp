#include "Stick.h"
#include <math.h>
#include <iostream>
 void Stick::update(int direction,int powerIn){
 	switch(direction){
 		case 0:
 			break;
 		case 1:
 			angle += 1;
 			break;
 		case -1:
 			angle -= 1;
 			break;
 		default:
 			break;
 	}
 	switch(powerIn){
 		case 0:
 			break;
 		case 1:
 			force += 1;
 			break;
 		case -1:
 			force -= 1;
 			break;
 		default:
 			break;
 	}
 	double cosTheta,sinTheta;
 	if (angle < 0)
 	{
 		angle += 360;
 	}
 	angle = angle - 360*((int)angle/360);
 	cosTheta = cos(angle*3.14529/180.0);
 	sinTheta = sqrt(1-pow(cosTheta,2));
 	if(angle <= 360 && angle >= 180 ){
 		sinTheta = -sinTheta;
 	}
 	power.z = force*cosTheta;
 	power.x = force*sinTheta;
 	//std::cout << power.z << ":z=power" <<std::endl;
 	//std::cout << power.x << ":x=power" <<std::endl;
 	//std::cout << angle << ":=angle" <<std::endl;

 	pos.x = target.x + power.x;
 	pos.z = target.z + power.z;
 }

void Stick::updateTarget(point p){
	target = p;
}

