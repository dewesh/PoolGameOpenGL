#include "Stick.h"
#include <math.h>
#include <iostream>

Stick::Stick(){
	length = 10;
}
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
//**************************************************************
void Stick::updateTarget(point p){
	target = p;
}
//**************************************************************
int Stick::animate(){
	if(count == 10){
		count = 0;
		return 0;
	}
	else{
		pos.x -= power.x/10;
		pos.z -= power.z/10;
		count ++;
		return 1;
	}
}
//**************************************************************
void Stick::reset(){
	angle = 0 ;
	pos.x = -1000;
	pos.z = 0;
	force = 0;
	power.x =power.z =0;
}