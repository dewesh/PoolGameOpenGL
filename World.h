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
 	void update();
 	void reset();
 	void updateCollisionBall(Ball,Ball);
 	bool isCollision(Ball,Ball);
 };

#endif