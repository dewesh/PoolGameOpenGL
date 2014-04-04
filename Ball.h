 #ifndef BALL_H
#define BALL_H

 #include "Game.h"

 class Ball
 {
 public:
 	point location;
 	point velocity,acc;
 	double radius;
 	Ball();
 	Ball(double);
 	void update();
 	void render();
 	/* data */
 };
#endif
