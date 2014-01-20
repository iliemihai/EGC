#include "camera.h"

Camera::Camera(){
}
Camera::~Camera(){
}

void Camera::init(){
	position = Vector3D(0,0,3);
	forward = Vector3D(0,0,-1);
	up = Vector3D(0,1,0);
	right = Vector3D(1,0,0);
}

void Camera::translate_Forward(float dist){
	position += forward * dist;
}
void Camera::translate_Up(float dist){
	position += up * dist;
}
void Camera::translate_Right(float dist){
	position += right * dist;
}




void Camera::rotateFPS_OY(float angle){
	forward = forward * cos(angle) + right * sin(angle);
	right = forward.CrossProduct(up);
}
void Camera::rotateFPS_OX(float angle){
	up = up * cos(angle) + forward * sin(angle);
	forward = up.CrossProduct(right);
}
void Camera::rotateFPS_OZ(float angle){
	right = right * cos(angle) + up * sin(angle);
	up = right.CrossProduct(forward);
}
void Camera::rotateTPS_OY(float angle, float dist_to_interes){
	translate_Forward(dist_to_interes);
	rotateFPS_OY(angle);
	translate_Forward(-dist_to_interes);

}
void Camera::rotateTPS_OX(float angle, float dist_to_interes){
	translate_Forward(dist_to_interes);
	rotateFPS_OX(angle);
	translate_Forward(-dist_to_interes);
}
void Camera::rotateTPS_OZ(float angle, float dist_to_interes){
	translate_Forward(dist_to_interes);
	rotateFPS_OZ(angle);
	translate_Forward(-dist_to_interes);
}


void Camera::render(){
	Vector3D center = position + forward;
	gluLookAt(	position.x, position.y, position.z, 
				center.x, center.y, center.z,
				up.x, up.y, up.z);
}