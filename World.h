#ifndef WORLD_H
#define WORLD_H

#include "Ball.h"
#include "Camera.h"
#include "Stick.h"
#include "Game.h"
 
 class World
 {
 	
 public:
 	enum STATE _STATE;
 	Ball ball[15];
 	Ball qBall;
 	Camera *camera;
 	int camFar;
 	Stick stick;
 	double collisionFriction;	// ball ball collision
 	World();
 	void update();
 	void reset();
 	double getPhi(Ball,Ball);
 	bool isCollision(Ball *,Ball *);
 	void updateBallCollision(Ball *, Ball *,int ,int);
 };

#endif
