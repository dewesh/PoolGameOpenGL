 #ifndef BALL_H
#define BALL_H

 #include "Game.h"

 class Ball
 {
 public:
 	point pos;
 	point velocity,acc;
 	double radius;
 	Ball();
 	Ball(double);
 	void update();
 	void reset();
 };
#endif
