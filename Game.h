#ifndef GAME_H
#define GAME_H

#include <vector>

using namespace std;
enum STATE { START,POSITIONSTICK,TAKESHOT,READY,HIT,RUNNING,STOPPED };
enum PLAYER_TURN { fFIRST, SECOND };
enum SCREEN { INTRODUCTION,MENU,GAME,END };
enum BALL_TYPE {SOLID, STRIPE,QBALL,BLACK };

class point {
public:
	double x,y,z;
};

typedef struct {
    int width;
	int height;
	char* title;
	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;

#endif
