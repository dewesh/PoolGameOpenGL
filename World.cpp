#include "World.h"

World::World(){
 	// set all initial declaration
 }
 //**************************************************************
 void World::update(){
 	for (int i = 0; i < 15; ++i)
 	{
 		ball[i].update();
 	}
 }

 //**************************************************************
 void World::render(){

 }
