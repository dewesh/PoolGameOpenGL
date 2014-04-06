#include "World.h"
#include <math.h>
#include <iostream>
#include <stdio.h>



World::World(){
	
	qBall.pos.x = -50;
	qBall.pos.z = 0;
	collisionFriction = 1.01;
	stick.updateTarget(qBall.pos);
	reset();
}

void World::updateBallCollision(Ball *a, Ball *b,int u,int v)
{
	
	if(!( a -> previousCollison == v && b -> previousCollison == u)){
		if(isCollision(a,b)){
			
			printf("Collision : %d:%d,  ,%d:%d\n",a -> previousCollison,u,b -> previousCollison,v);
			a -> previousCollison = v;
			b -> previousCollison = u;
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
			a->velocity.x = (alongB1*cosPhi + normalB1*sinPhi)/collisionFriction ;
			a->velocity.z = (-alongB1*sinPhi + normalB1*cosPhi)/collisionFriction;
			b->velocity.x = (alongB2*cosPhi + normalB2*sinPhi)/collisionFriction;
			b->velocity.z = (-alongB2*sinPhi + normalB2*cosPhi)/collisionFriction;

		}
	}
}

//**************************************************************
 void World::update(){
 	if(_STATE == START){
 		reset();
 		_STATE = POSITIONSTICK;
 		std::cout << "POSITIONSTICK" << std::endl ;
 	}
 	else if (_STATE == POSITIONSTICK)
 	{
 		//click will trigger the hit button
 	}
 	else if (_STATE == TAKESHOT)
 	{
 		if(stick.animate() == 0){
 			_STATE =HIT;
 			std::cout << "HIT" << std::endl ;

 		}
 		
 	}
 	else if(_STATE == HIT){
 		// update queball and change state
		qBall.velocity.z = -stick.power.z/10;
		qBall.velocity.x = -stick.power.x/10;
		stick.reset();
 		_STATE = RUNNING;
 		std::cout << "RUNNING" << std::endl ;
 	}
 	else if (_STATE == RUNNING)
 	{
 		
	 
 		std::cout << "RUNNING" << std::endl ;
 		for (int i = 0; i < 15; ++i)
 		{
 			if(qBall.velocity.x <= 0.005 && qBall.velocity.x >= -0.005 && qBall.velocity.z <= 0.005 && qBall.velocity.z >= -0.005){
	 			ball[i].velocity.x = 0;
	 			ball[i].velocity.z = 0;
	 			_STATE = READY;
	 			continue;
	 		}
	 		_STATE = RUNNING;
	 		break;
 		}
 		if(qBall.velocity.x <= 0.005 && qBall.velocity.x >= -0.005 && qBall.velocity.z <= 0.005 && qBall.velocity.z >= -0.05 && _STATE == READY){
 			qBall.velocity.x = 0;
 			qBall.velocity.z = 0;
 		}
 		else{
 			_STATE = RUNNING;
 		}
 		//call update
 		camera.update();
	 	qBall.update();
 		for (int i = 0; i < 15; ++i)
	 	{
	 		ball[i].update();
	 		updateBallCollision(&qBall,&ball[i],15,i);
	 		for (int j = i+1; j < 15; ++j)
	 		{
	 			updateBallCollision(&ball[i],&ball[j],i,j);
	 		}
	 	}
 	}
 	else if (_STATE == READY)
 	{
    	stick.updateTarget(qBall.pos);
 		stick.update(0,0);
 		std::cout << "RUNNING" << std::endl ;

 	}
 	else{// stopped

 	}
 	
 }
 //**************************************************************
 void World::reset(){
 	for (int i = 0; i < 15; ++i)
	{
		ball[i].pos.x = i*3;
		ball[i].pos.z = i*3;
		ball[i].reset();
	}
	qBall.reset();
 }
//**************************************************************
 double World::getCosPhi(Ball b1,Ball b2){
 	double dist;
 	dist = ((b2.pos.x-b1.pos.x)*(b2.pos.x-b1.pos.x)) + ((b2.pos.y-b1.pos.y)*(b2.pos.y-b1.pos.y)) + ((b2.pos.z-b1.pos.z)*(b2.pos.z-b1.pos.z));
 	return (b1.pos.x-b2.pos.x)/sqrt(dist);
 }
//**************************************************************
bool World::isCollision(Ball *b1,Ball *b2){
	double dist,requiredDist;
	double nextPredictDistance;
	requiredDist = (b2->radius+b1->radius);
	//nextPredictDistance = sqrt(pow((b1->velocity.x+b2->velocity.x),2)+pow((b1->velocity.z+b2->velocity.z),2));
	//requiredDist+=nextPredictDistance;
	dist = sqrt(((b2->pos.x-b1->pos.x)*(b2->pos.x-b1->pos.x)) + ((b2->pos.y-b1->pos.y)*(b2->pos.y-b1->pos.y)) + ((b2->pos.z-b1->pos.z)*(b2->pos.z-b1->pos.z)));
	if(dist <= requiredDist){
		
		return true;
	}
	else{
		return false;
	}
}
