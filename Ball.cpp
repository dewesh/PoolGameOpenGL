#include "Ball.h"

Ball::Ball(){
	radius = 1.2;
	pos.y = radius;
	pos.x = 0;
	pos.z = 0;
	previousCollison = -1;
}
Ball::Ball(double radius)
{
 	pos.y = radius;
 	radius = radius;
}
 
 
void Ball::update()
{
	velocity.x = velocity.x/1.002;
	velocity.z = velocity.z/1.002; 
	pos.x += velocity.x;
	pos.z += velocity.z;
}

void Ball::reset(){
	velocity.x = 0;
	velocity.z = 0;
	velocity.y = 0;
	previousCollison = -1;

}
