#ifndef WORLD_H
#define WORLD_H

#include "Ball.h"
#include "Camera.h"
#include "Stick.h"
#include "Game.h"
 
 class World
 {
 	
 public:
 	Ball ball[15];
 	Ball qBall;
 	Camera camera;
 	Stick stick;
 	int collisionEntry[16];
 	World();
 	void update();
 	void reset();
 	double getCosPhi(Ball,Ball);
 	bool isCollision(Ball *,Ball *);
 	
 	void updateBallCollision(Ball *, Ball *,int ,int);
 };

#endif
