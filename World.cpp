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

void World::updateBallCollision(Ball *a, Ball *b)
{
	if(isCollision(a,b)){
			//update b1 and b2
		double alongB1,alongB2,normalB1,normalB2;
		double cosPhi,sinPhi;
		cosPhi = getCosPhi(*a,*b);
		sinPhi = sqrt(1-pow(cosPhi,2));
		alongB2 = b->velocity.x * cosPhi - b->velocity.z * sinPhi;
		normalB2 = b->velocity.x * sinPhi + b->velocity.z * cosPhi;
		alongB1 = a->velocity.x * cosPhi - a->velocity.z * sinPhi;
		normalB1 = a->velocity.x * sinPhi + a->velocity.z * cosPhi;
		//change to along and normal
		//swap along
		double temp;
		temp = alongB2;
		alongB2 = alongB1;
		alongB1 = temp;
		//change to x and y
		a->velocity.x = alongB1*cosPhi + normalB1*sinPhi;
		a->velocity.z = -alongB1*sinPhi + normalB1*cosPhi;
		b->velocity.x = alongB2*cosPhi + normalB2*sinPhi;
		b->velocity.z = -alongB2*sinPhi + normalB2*cosPhi;

	}

}


 void World::update(){
 	qBall.update();
 	for (int i = 0; i < 15; ++i)
 	{
 		ball[i].update();
 		updateBallCollision(&qBall,&ball[i]);
 		for (int j = i+1; j < 15; ++j)
 		{
 			updateBallCollision(&ball[i],&ball[j]);
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

bool World::isCollision(Ball *b1,Ball *b2){
	double dist,requiredDist;
	double nextPredictDistance;
	requiredDist = (b2->radius+b1->radius);
	nextPredictDistance = sqrt(pow((b1->velocity.x+b2->velocity.x),2)+pow((b1->velocity.z+b2->velocity.z),2));
	requiredDist+=nextPredictDistance;
	dist = sqrt(((b2->pos.x-b1->pos.x)*(b2->pos.x-b1->pos.x)) + ((b2->pos.y-b1->pos.y)*(b2->pos.y-b1->pos.y)) + ((b2->pos.z-b1->pos.z)*(b2->pos.z-b1->pos.z)));
	if(dist <= requiredDist){
		
		return true;
	}
	else{
		return false;
	}
}
