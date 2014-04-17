#include "World.h"
#include <math.h>
#include <iostream>
#include <stdio.h>


#define root2 sqrt(2)

//**************************************************************
World::World(){
	_SCREEN = GAME;// update when all the screens added
	camFar = 20;
	qBall.pos.x = -50;
	qBall.pos.z = 0;
	qBall.Balltype='W';
	collisionFriction = 1;
	stick.updateTarget(qBall.pos);
	stick.update(0,0);
	point camPos;
	camPos.x = stick.target.x + stick.length*stick.sinTheta*camFar;
	camPos.z = stick.target.z + stick.length*stick.cosTheta*camFar;
	camPos.y = stick.length*camFar;
	camera = new Camera(camPos,qBall.pos);
	FOUL=false;
	activePlayer=0;
	defineBallTypes();
	table = new Table(300,150);
	for (int i = 0; i < 4; ++i)
	{
		holes[i].radius = 15;
		holes[i].pos.y = holes[i].radius;
	}
	holes[0].pos = table->p1;
	holes[1].pos = table->p2;
	holes[2].pos = table->p3;
	holes[3].pos = table->p4;
	reset();
}

//**************************************************************
 void World::update(){
 	int iter;
 
 	if(_STATE == START){
 		reset();
 		_STATE = POSITIONSTICK;
 		std::cout << "POSITIONSTICK" << std::endl ;
 	}
 	else if (_STATE == POSITIONSTICK)
 	{
 		if(!qBall.active){	
			qBall.pos.x = 0;
			qBall.pos.z = table->len*0.25;
			qBall.reset();
			qBall.pos.y = qBall.radius;
			qBall.active = true;
			stick.updateTarget(qBall.pos);
			stick.update(0,0);
			//_STATE = READY;
		}
 		//click will trigger the hit button
		point camPos;
		camPos.x = stick.target.x + stick.length*stick.sinTheta*camFar;
		camPos.z = stick.target.z + stick.length*stick.cosTheta*camFar;
		camPos.y = stick.length*camFar;
		//std::cout << camPos.x << std::endl;
		camera->setCamera(camPos,qBall.pos);
		//std::cout << "POSITIONSTICK" << std::endl ;

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
		qBall.velocity.z = -stick.power.z/5;
		qBall.velocity.x = -stick.power.x/5;
		//qBall.firstCollision=false;
		stick.reset();
 		_STATE = RUNNING;
 		std::cout << "RUNNING" << std::endl ;
 	}
 	else if (_STATE == RUNNING)
 	{
 		//std::cout << "RUNNING" << std::endl ;
 		for (int i = 0; i < 15; ++i)
 		{
 			if(ball[i].velocity.x <= 0.005 && ball[i].velocity.x >= -0.005 && ball[i].velocity.z <= 0.005 && ball[i].velocity.z >= -0.005){
	 			ball[i].reset();
	 			_STATE = READY;
	 			continue;
	 		}
	 		_STATE = RUNNING;
	 		break;
 		}
 		if(qBall.velocity.x <= 0.005 && qBall.velocity.x >= -0.005 && qBall.velocity.z <= 0.005 && qBall.velocity.z >= -0.05 && _STATE == READY){
 			qBall.reset();
 		}
 		else{
 			_STATE = RUNNING;
 		}
 		//call update
	 	qBall.update();
		if(checkHole(&qBall)==true)	// change the state to place ball 
		FOUL=true;
		
 		for (int i = 0; i < 15; ++i)
	 	{
	 		ball[i].update();
	 		updateBallCollision(&qBall,&ball[i],15,i);
	 		if(checkHole(&ball[i])){
	 			ball_pocketed=1;
	 			updatePlayerInfo(ball[i],activePlayer);
	 			cout << "ball::" << i << " pocketed" <<endl;
	 			if(player[0].balltype==ball[i].Balltype)
	 			{
	 				player[0].incrementScore();
	 				ball[i].pos.z = -(table->len/2+table->thickness + qBall.radius*2);
	 				ball[i].pos.x = table->bre/2+table->thickness-qBall.radius*3*player[0].score;
	 			}
	 			else{
	 				player[1].incrementScore();
	 				ball[i].pos.z = (table->len/2+table->thickness + qBall.radius*2);
	 				ball[i].pos.x = table->bre/2+table->thickness-qBall.radius*3*player[1].score;
	 			}
	 			ball[i].active=false;
	 			//if this is the first hole then only update the player
	 			
	 		}
	 		for (int j = i+1; j < 15; ++j)
	 		{
	 			updateBallCollision(&ball[i],&ball[j],i,j);
	 		}
	 	}
	 	
	 	for(iter=0;iter<8;iter++)
	 	{
	 		if(ball[iter].active)
	 		break;
	 	}
	 	//cout << "Active RED Ball " << iter << "\n";
	 	if(iter==8)
	 	{
	 		if('R'==player[0].balltype)
	 		cout << "Player 0 Won\n";
	 		
	 		else
	 		cout << "Player 1 Won\n";
	 		
	 		_STATE = START;
	 		_SCREEN = END; 
	 	}
	 	
	 	for(iter=8;iter<15;iter++)
	 	{
	 		if(ball[iter].active)
	 		break;
	 	}
	 	//cout << "Active YELLOW Ball  " << iter <<"\n";
	 	if(iter==15)
	 	{
	 		if('Y'==player[0].balltype)
	 		cout << "Player 0 Won\n";
	 		
	 		else
	 		cout << "Player 1 Won\n";
	 		
	 		_STATE=START;
	 		_SCREEN = END; 
	 	}
	 	
	 	
 	}
 	else if (_STATE == READY)
 	{
    	stick.updateTarget(qBall.pos);
 		stick.update(0,0);
 		qBall.previousCollison = -1;

 		if(ball_pocketed==0 || FOUL==true)
		 	SwitchPlayer();
		 	FOUL=false;
	 	printf("Active Player at table is player-%d\n",activePlayer);
		ball_pocketed =0 ;
 		_STATE = POSITIONSTICK;
 		std::cout << "READY" << std::endl ;
 	}
 	else{// stopped

 	}
 	
 }
 //**************************************************************
 void World::reset(){
 	for (int i = 0; i < 15; ++i)
	{
		ball[i].reset();
		ball[i].active = true;
		ball[i].pos.y = ball[i].radius;
	}
	ball[0].pos.x = 0;
	ball[0].pos.z = -table->len*0.25+ball[0].radius*0.5;
	ball[1].pos.x = 0;
	ball[1].pos.z = -table->len*0.25-ball[0].radius*2.2*root2;
	ball[2].pos.x = 0;
	ball[2].pos.z = -table->len*0.25-ball[0].radius*4.4*root2;
	ball[3].pos.x = ball[0].radius*1.1*root2;
	ball[3].pos.z = -table->len*0.25 -ball[0].radius*1.1*root2;
	ball[4].pos.x = ball[0].radius*1.1*root2;
	ball[4].pos.z = -table->len*0.25 -ball[0].radius*3.3*root2;
	ball[5].pos.x = -ball[0].radius*1.1*root2;
	ball[5].pos.z = -table->len*0.25 -ball[0].radius*1.1*root2;
	ball[6].pos.x = -ball[0].radius*1.1*root2;
	ball[6].pos.z = -table->len*0.25 -ball[0].radius*3.3*root2;
	ball[7].pos.x = ball[0].radius*2.2*root2;
	ball[7].pos.z = -table->len*0.25-ball[0].radius*2.2*root2;
	ball[8].pos.x = ball[0].radius*2.2*root2;
	ball[8].pos.z = -table->len*0.25-ball[0].radius*4.4*root2;
	ball[9].pos.x = -ball[0].radius*2.2*root2;
	ball[9].pos.z = -table->len*0.25-ball[0].radius*2.2*root2;
	ball[10].pos.x = -ball[0].radius*2.2*root2;
	ball[10].pos.z = -table->len*0.25-ball[0].radius*4.4*root2;
	ball[11].pos.x = ball[0].radius*3.3*root2;
	ball[11].pos.z = -table->len*0.25-ball[0].radius*3.3*root2;
	ball[12].pos.x = -ball[0].radius*3.3*root2;
	ball[12].pos.z = -table->len*0.25-ball[0].radius*3.3*root2;
	ball[13].pos.x = ball[0].radius*4.4*root2;
	ball[13].pos.z = -table->len*0.25-ball[0].radius*4.4*root2;
	ball[14].pos.x = -ball[0].radius*4.4*root2;
	ball[14].pos.z = -table->len*0.25-ball[0].radius*4.4*root2;
	qBall.pos.x = 0;
	qBall.pos.z = table->len*0.25;
	qBall.reset();
	qBall.pos.y = qBall.radius;
	qBall.active = true;
	stick.updateTarget(qBall.pos);
	stick.update(0,0);
	point camPos;
	camPos.x = stick.target.x + stick.length*stick.sinTheta*camFar;
	camPos.z = stick.target.z + stick.length*stick.cosTheta*camFar;
	camPos.y = stick.length*camFar;

	camera->setCamera(camPos,qBall.pos);
 }
//**************************************************************
 double World::getPhi(Ball b1,Ball b2){
 	//double dist;
 	//dist = ((b2.pos.x-b1.pos.x)*(b2.pos.x-b1.pos.x)) + ((b2.pos.y-b1.pos.y)*(b2.pos.y-b1.pos.y)) + ((b2.pos.z-b1.pos.z)*(b2.pos.z-b1.pos.z));
 	double phi;
 	phi = atan2(b2.pos.z - b1.pos.z  , - b2.pos.x + b1.pos.x);
 	// std::cout << phi << std::endl;
 	// return (b1.pos.x-b2.pos.x)/sqrt(dist);
 	return phi;
 }
//**************************************************************
bool World::isCollision(Ball *b1,Ball *b2){
	double dist,requiredDist;
	double nextPredictDistance;
	if(b1->active && b2->active){
		requiredDist = (b2->radius+b1->radius);
		dist = sqrt(((b2->pos.x-b1->pos.x)*(b2->pos.x-b1->pos.x)) + ((b2->pos.y-b1->pos.y)*(b2->pos.y-b1->pos.y)) + ((b2->pos.z-b1->pos.z)*(b2->pos.z-b1->pos.z)));
		if(dist <= requiredDist){
			
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}
//**************************************************************
void World::defineBallTypes()
{
	int i;
	for(i=0;i<8;i++)
	ball[i].Balltype='R';
	
	for(i=8;i<15;i++)
	ball[i].Balltype='Y';
}
//**************************************************************
void World::SwitchPlayer()
{
	if(activePlayer==1)
	activePlayer=0;
	else
	activePlayer=1;
}
//**************************************************************
void World::updatePlayerInfo(Ball b,int activePlayer)              // Called if a player picks a ball
{
	if(player[activePlayer].balltype=='W')
	{
	char inverseBallColor;
	char color=b.Balltype;
	if(b.Balltype=='R')
	inverseBallColor='Y';
	else
	inverseBallColor='R';
	
	player[activePlayer].setballtype(color);
	if(activePlayer)
	player[0].setballtype(inverseBallColor);
	else
	player[1].setballtype(inverseBallColor);
	}
}
//**************************************************************
bool World::updateBallCollision(Ball *a, Ball *b,int u,int v)
{
	
	if(!( a -> previousCollison == v && b -> previousCollison == u)){
		if(isCollision(a,b)){
		
			if(a->firstCollision==false && a==&qBall)
			{
			a->firstCollision=true;
				if((player[activePlayer].balltype!=b->Balltype && player[activePlayer].balltype!='W') || b==&ball[0])
				{
								
				FOUL=true;
				}
			}
			
			// printf("Collision : %d:%d,  ,%d:%d\n",a -> previousCollison,u,b -> previousCollison,v);
			a -> previousCollison = v;
			b -> previousCollison = u;
				//update b1 and b2
			double alongB1,alongB2,normalB1,normalB2;
			double cosPhi,sinPhi,Phi;
			Phi = getPhi(*a,*b);
			cosPhi = cos(Phi);
			sinPhi = sin(Phi);
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
			return true;
		}
		else{
			return false;
		}
	}
}
//**************************************************************
bool World::checkHole(Ball *b1){
	double dist,requiredDist;
	double nextPredictDistance;
	bool isPocket = false;
	if(b1->active)
	for (int i = 0; i < 4; ++i)
	{
		requiredDist = (holes[i].radius);
		dist = sqrt(((holes[i].pos.x-b1->pos.x)*(holes[i].pos.x-b1->pos.x)) + ((holes[i].pos.y-b1->pos.y)*(holes[i].pos.y-b1->pos.y)) + ((holes[i].pos.z-b1->pos.z)*(holes[i].pos.z-b1->pos.z)));
		if(dist <= requiredDist){
			b1->reset();
			//b1->pos.y = -b1->radius*2;
			
			b1->active = false;
			isPocket = true;
			break;
		}
	}
	return isPocket;
}
