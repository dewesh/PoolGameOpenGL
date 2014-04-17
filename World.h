#ifndef WORLD_H
#define WORLD_H

#include "Ball.h"
#include "Camera.h"
#include "Stick.h"
#include "Game.h"
#include "Table.h"
#include "Player.h"
#include <vector>
 
 class World
 {
 	
 public:
 	enum STATE _STATE;
 	enum SCREEN _SCREEN;
 	Ball ball[15];
 	Ball qBall;
 	Ball holes[4];
 	Player player[2];
 	bool FOUL;
 	int activePlayer;
 	Camera *camera;
 	enum PLAYER_TURN _WINNER ;
 	Table *table;
 	int camFar;
 	Stick stick;
 	int ball_pocketed;
 	double collisionFriction;	// ball ball collision
 	World();
 	void update();
 	void reset();
 	double getPhi(Ball,Ball);
 	bool isCollision(Ball *,Ball *);
 	bool isNextCollision(Ball *,Ball *);
 	bool checkHole(Ball*);
 	bool updateBallCollision(Ball *, Ball *,int ,int);
 	void defineBallTypes();
 	void updatePlayerInfo(Ball ,int);
 	void SwitchPlayer();
 };

#endif
