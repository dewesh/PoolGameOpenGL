#include<stdio.h>

#include "Player.h"

Player::Player()
	{
	//Playername=name;
	balltype='W';
	score=0;
	}
	
	
void Player::setballtype(char ball)
{
	balltype=ball;
}

void Player::incrementScore()
{
	score+=1;
}

int Player::getScore()
{
	return score;
}
