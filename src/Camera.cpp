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

	this->fovy = fovy * M_PI / 180;
	this->fovx = this->fovy * ((float)width/(float)height);
	std::cout << "fovy " << this->fovy << " fovx " << this->fovx << std::endl;
	this->height = height;
	this->width = width;

	//calculate the u,v,w

	Vec3f positionMinusCenter = position - look;

	w = Vec3f::normalize(positionMinusCenter);
	u = Vec3f::normalize(Vec3f::cross(up, w));
	v = Vec3f::cross(w, u);

	//std::cout << "w: " << w << std::endl;
	//std::cout << "u: " << u << std::endl;
	//std::cout << "v: " << v << std::endl;

}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

Ray Camera::getRay(int x, int y) {
	//Calculate the direction
	//since grader wants pixel centers, we will add 0.5 to pixels.

	float horizontalChange = tan(fovy / 2) * (((float)x + 0.5f - ((float)height / 2)) / ((float) height / 2));

	float verticalChange = tan(fovx / 2) * ((((float) width/ 2) - ((float)y + 0.5f)) / ((float) width / 2));
	//std::cout << "for " << x << ", " << y << " horizontal change is "<< horizontalChange << " vertical change is "<< verticalChange << std::endl;
	Vec3f direction = (horizontalChange * u) + (verticalChange * v) - w;
	//Vec3f direction = (verticalChange * u) + (horizontalChange* v) - w;
	//Vec3f directionPart = verticalChange * v;

	direction = Vec3f::normalize(direction);

	//std::cout << "the for u(" << u.x << "," << u.y << "," << u.z << ")" << " ray part is (" << direction.x << "," << direction.y << "," << direction.z << ")" << std::endl;


	Ray temp(position, direction, 0, 100);
	return temp;

}
