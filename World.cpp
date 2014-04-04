#include "World.h"


 void World::update(){
 	for (int i = 0; i < 15; ++i)
 	{
 		ball[i].update();
 		for (int j = i+1; j < 15; ++j)
 		{
 			if(isCollision(ball[i],ball[j])){
				//update b1 and b2
				point temp;
				temp = ball[j].velocity;
				ball[j].velocity = ball[i].velocity;
				ball[i].velocity = temp;
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
 		ball[i].reset();
 	}
 }

void World::updateCollisionBall(Ball b1,Ball b2){
	
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