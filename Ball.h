 #ifndef BALL_H
#define BALL_H

 #include "Game.h"
#define PlayerTurn -1
#define BallRolling 1
 class Ball
 {
 public:
 	point pos;
 	point velocity,acc;
 	double radius;
 	int previousCollison;
 	Ball();
 	Ball(double);
 	void update();
 	void reset();
 };
#endif
