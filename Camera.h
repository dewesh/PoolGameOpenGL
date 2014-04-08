#ifndef CAMERA_H
#define CAMERA_H

 #include "Game.h"

 class Camera
 {
 public:
 	point cameraFrom;
 	point cameraTo;
 	Camera(point,point);
 	void setCamera(point,point);
 	void update(int,int);
 };
#endif
 
