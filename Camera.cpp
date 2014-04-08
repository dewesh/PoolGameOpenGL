#include "Camera.h"

Camera::Camera(point p1, point p2){
	cameraFrom = p1;
	cameraTo = p2;
}

void Camera::setCamera(point p1, point p2){
	cameraFrom = p1;
	cameraTo = p2;
}
void Camera::update(int mouseX,int mouseZ){

}