 #ifndef BALL_H
#define BALL_H

 #include "Game.h"

 class Ball
 {
 public:
 	point pos;
 	char Balltype;
 	point velocity,acc;
 	double radius;
 	int previousCollison;
 	bool active;
 	Ball();
 	Ball(double);
 	void update();
 	void reset();
 };
#endif
