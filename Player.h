#ifndef PLAYER_H
#define PLAYER_H
#include "Game.h"

class Player{
	
	public:
		char* Playername;
		char balltype;
		int score;
		void setballtype(char s);
		Player();
		void incrementScore();
		int getScore();
				
};
#endif
