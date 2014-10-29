/*
 * Camera.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Camera.h"
#include <iostream>

Camera::Camera(float lookfromx, float lookfromy, float lookfromz, float lookatx,
		float lookaty, float lookatz, float upx, float upy, float upz,
		float fovy, int height, int width) {
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
	this->fovx = fovy * (width / height);

	this->height = height;
	this->width = width;

	//calculate the u,v,w

	Vec3f positionMinusCenter = position - look;

	w = Vec3f::normalize(position);
	u = Vec3f::normalize(Vec3f::cross(up, w));
	v = Vec3f::cross(w, u);

	std::cout << "w: " << w.x << ", " << w.y << ", " << w.z << std::endl;
	std::cout << "u: " << u.x << ", " << u.y << ", " << u.z << std::endl;
	std::cout << "v: " << v.x << ", " << v.y << ", " << v.z << std::endl;

}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

Ray Camera::getRay(int x, int y) {
	//Calculate the direction
	//since grader wants pixel centers, we will add 0.5 to pixels.

	float horizontalChange = tan(fovx / 2) * (((float)x + 0.5f - (width / 2)) / (width / 2));
	float verticalChange = tan(fovy / 2) * (((height / 2) - (y + 0.5f)) / (height / 2));

	Vec3f direction = (horizontalChange * u + verticalChange * v - w);

	Ray temp(position, direction, 0, 100);
	return temp;

}
