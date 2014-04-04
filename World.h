#ifndef WORLD_H
#define WORLD_H

#include "Ball.h"
#include "Camera.h"
#include "Stick.h"
 
 class World
 {
 	
 public:
 	Ball ball[15];
 	Ball qBall;
 	Camera camera;
 	World();
 	void update();
 	void render();
 };

#endif