#include "Game.h"

 class Ball
 {
 private:
 	point location;
 	point velocity,acc;
 	double radius;
 public:

 	Ball(double);
 	void update();
 	void render();
 	/* data */
 };