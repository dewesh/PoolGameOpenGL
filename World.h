#include "Game.h" 
#include "Ball.h"
#include "Camera.h"
#include "Stick.h"
#include<iostream>

 class World
 {
 	Ball ball[15];
 	Ball qBall;
 	Camera camera;
 public:
 	World();
 	void update();
 	void render();
 };
