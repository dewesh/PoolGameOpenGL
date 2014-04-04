#include "World.h"
#include<math.h>

World::World(){
	for (int i = 0; i < 15; ++i)
	{
		ball[i].pos.x = i*2;
		ball[i].pos.z = i*2;
	}
	qBall.pos.x = -10;
	qBall.pos.z = 0;
}
 void World::update(){
 	qBall.update();
 	for (int i = 0; i < 15; ++i)
 	{
 		ball[i].update();
 		if(isCollision(qBall,ball[i])){
			//update b1 and b2
			double alongB1,alongB2,normalB1,normalB2;
			double cosPhi,sinPhi;
			cosPhi = getCosPhi(qBall,ball[i]);
			sinPhi = sqrt(1-pow(cosPhi,2));
			alongB2 = ball[i].velocity.x * cosPhi - ball[i].velocity.z * sinPhi;
			normalB2 = ball[i].velocity.x * sinPhi + ball[i].velocity.z * cosPhi;
			alongB1 = qBall.velocity.x * cosPhi - qBall.velocity.z * sinPhi;
			normalB1 = qBall.velocity.x * sinPhi + qBall.velocity.z * cosPhi;
			//change to along and normal
			//swap along
			double temp;
			temp = alongB2;
			alongB2 = alongB1;
			alongB1 = temp;
			//change to x and y
			qBall.velocity.x = alongB1*cosPhi + normalB1*sinPhi;
			qBall.velocity.z = -alongB1*sinPhi + normalB1*cosPhi;
			ball[i].velocity.x = alongB2*cosPhi + normalB2*sinPhi;
			ball[i].velocity.z = -alongB2*sinPhi + normalB2*cosPhi;

		}
 		for (int j = i+1; j < 15; ++j)
 		{
 			if(isCollision(ball[i],ball[j])){
				//update b1 and b2
				double alongB1,alongB2,normalB1,normalB2;
				double cosPhi,sinPhi;
				cosPhi = getCosPhi(ball[i],ball[j]);
				sinPhi = sqrt(1-pow(cosPhi,2));
				alongB2 = ball[j].velocity.x * cosPhi - ball[j].velocity.z * sinPhi;
				normalB2 = ball[j].velocity.x * sinPhi + ball[j].velocity.z * cosPhi;
				alongB1 = ball[i].velocity.x * cosPhi - ball[i].velocity.z * sinPhi;
				normalB1 = ball[i].velocity.x * sinPhi + ball[i].velocity.z * cosPhi;
				//change to along and normal
				//swap along
				double temp;
				temp = alongB2;
				alongB2 = alongB1;
				alongB1 = temp;
				//change to x and y
				ball[i].velocity.x = alongB1*cosPhi + normalB1*sinPhi;
				ball[i].velocity.z = -alongB1*sinPhi + normalB1*cosPhi;
				ball[j].velocity.x = alongB2*cosPhi + normalB2*sinPhi;
				ball[j].velocity.z = -alongB2*sinPhi + normalB2*cosPhi;

			}
 		}
 	}
 	camera.update();
 	stick.update();
 	qBall.update();
 }
 
 void World::reset(){
 	for (int i = 0; i < 15; ++i)
	{
		ball[i].pos.x = i*2;
		ball[i].pos.z = i*2;
	}
 }

 double World::getCosPhi(Ball b1,Ball b2){
 	double dist;
 	dist = ((b2.pos.x-b1.pos.x)*(b2.pos.x-b1.pos.x)) + ((b2.pos.y-b1.pos.y)*(b2.pos.y-b1.pos.y)) + ((b2.pos.z-b1.pos.z)*(b2.pos.z-b1.pos.z));
 	return (b1.pos.x-b2.pos.x)/sqrt(dist);
 }

bool World::isCollision(Ball b1,Ball b2){
	double dist,requiredDist;
	requiredDist = (b2.radius+b1.radius)*(b2.radius+b1.radius);
	dist = ((b2.pos.x-b1.pos.x)*(b2.pos.x-b1.pos.x)) + ((b2.pos.y-b1.pos.y)*(b2.pos.y-b1.pos.y)) + ((b2.pos.z-b1.pos.z)*(b2.pos.z-b1.pos.z));
	if(dist <= requiredDist){
		return true;
	}
	else{
		return false;
	}
}