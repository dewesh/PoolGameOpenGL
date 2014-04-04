#include "Ball.h"

Ball::Ball(){
	radius = 1.2;
	pos.y = radius;
	pos.x = 0;
	pos.z = 0;
}
Ball::Ball(double radius)
{
 	pos.y = radius;
 	radius = radius;
}
 
 
void Ball::update()
{
	velocity.x = velocity.x;
	velocity.z = velocity.z; 
	pos.x += velocity.x;
	pos.z += velocity.z;
}

void Ball::reset(){

}
