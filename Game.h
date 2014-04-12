#ifndef GAME_H
#define GAME_H

#include <vector>

using namespace std;
enum STATE { START,POSITIONSTICK,TAKESHOT,READY,HIT,RUNNING,STOPPED };
enum PLAYER_TURN { fFIRST, SECOND };
enum BALL_TYPE {SOLID, STRIPE };

class point {
public:
	double x,y,z;
};

/*class Player{
public:
	int score;
	vector<int> balls;
	int name;
	int ballType;
};*/
#endif
