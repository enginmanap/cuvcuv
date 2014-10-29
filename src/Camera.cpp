/*
 * Camera.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Camera.h"

Camera::Camera(float lookfromx, float lookfromy, float lookfromz, float lookatx,
		float lookaty, float lookatz, float upx, float upy, float upz,
		float fovy) {
	position.x = lookfromx;
	position.y = lookfromy;
	position.z = lookfromz;

	look.x = lookatx;
	look.y = lookaty;
	look.z = lookatz;

	up.x = upx;
	up.y = upy;
	up.z = upz;

	this->fovy = fovy;

}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

